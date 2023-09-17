/* $Header: /usr/cvsroot/target/h/wrn/wm/demo/snmpconf.h,v 1.2 2002/03/11 19:23:36 josh Exp $ */

/*
 * Copyright (C) 1999-2005, 2012 Wind River Systems, Inc.
 * All rights reserved.  Provided under license only.
 * Distribution or other use of this software is only
 * permitted pursuant to the terms of a license agreement
 * from Wind River Systems (and is otherwise prohibited).
 * Refer to that license agreement for terms of use.
 */


/****************************************************************************
 *  Copyright 1991-1997 Epilogue Technology Corporation.
 *  Copyright 1998 Integrated Systems, Inc.
 *  All rights reserved.
 ****************************************************************************/

/* declarations for routines in parse.c */

/*
 * $Log: snmpconf.h,v $
 * Revision 1.2  2002/03/11 19:23:36  josh
 * change dest_addr to an ipaddr_t, and add definitions for ipaddr_t if
 * Attache isn't installed
 *
 * Revision 1.1.1.1  2001/11/05 17:47:37  tneale
 * Tornado shuffle
 *
 * Revision 1.4  2001/01/19 22:23:37  paul
 * Update copyright.
 *
 * Revision 1.3  2000/03/17 00:12:13  meister
 * Update copyright message
 *
 * Revision 1.2  1998/11/25 03:38:17  sra
 * "timeout" => "snmp_timeout" to fix OpEN symbol conflict.
 *
 * Revision 1.1  1998/06/23 20:45:04  sar
 * The gneral snmp configuration information, moved here from snmptalk/parse.h
 *
 * Revision 7.8  1998/06/07 02:52:55  sar
 * Move the global configuration arguments to snark library
 *
 * Revision 7.7  1998/05/23 18:19:25  sar
 * Added global arguments for v3 along with their defaults
 *
 * Revision 7.6  1998/02/25 04:58:01  sra
 * Update copyrights.
 *
 * Revision 7.5  1997/03/20 06:53:54  sra
 * DFARS-safe copyright text.  Zap!
 *
 * Revision 7.4  1997/02/25 10:58:16  sra
 * Update copyright notice, dust under the bed.
 *
 * Revision 7.3  1997/02/19 08:10:29  sra
 * More fun merging snmptalk into snark, general snark cleanup.
 *
 * Revision 7.2  1997/01/10 20:32:24  sra
 * Punt this copy of parse_line(), since there's a better one snark/lib.
 *
 * Revision 7.1  1997/01/08 01:58:11  sar
 * Removed no_pp stuff and updated copyrights
 *
 * Revision 7.0  1996/03/18  20:19:20  sar
 * Updated rev to 7.0 and copyright to 96
 *
 * Revision 6.3  1996/01/10  16:41:40  sar
 * Usec updates
 *
 * Revision 6.2  1995/11/11  00:09:41  sar
 * Added a new command option to decide where the rfc1445 viewtree
 * comes from - the config.txt file or nvviews.c
 *
 * Revision 6.1  1995/10/20  22:39:59  sar
 * remvoed dparty, sparty and context info
 *
 * Revision 6.0  1995/05/31  21:50:19  sra
 * Release 6.0.
 *
 * Revision 5.4  1995/03/21  00:50:48  sar
 * Added a packet_dump option to snmptalk, this allows the user to trun
 * packet dumping on & off while snmptalk is running.
 *
 * Revision 5.3  1994/11/09  01:43:52  sar
 * Read the nv config file if/when we need to.  If the master_agent or
 * v2_view options are installed we always read it, if they aren't vut
 * v2 is installed we check the snmptalk_version flag if it is 2 we read
 * the file if it is 1 we don't.  The first time we switch to v2 we
 * try and read the file and complain if one doesn't exist.  If we only
 * have v1 installed we never read the config file.
 *
 * Revision 5.2  1994/09/23  19:29:50  sar
 * Removed the old non volatile scheme and added a new one that is hopefully
 * easier to use and allows for some new options such as a read only
 * flag to prohibit creating a new config file.  The config stuff will
 * only be included if needed.
 * In snmptalk and snmpint added a next function.
 *
 * Revision 5.1  1994/07/24  15:41:03  dab
 * Added ability to set UDP port used by manager and the set command can
 * now specify enumerated integer values by symbolic name.
 *
 * Revision 5.0  1994/05/16  15:57:40  sar
 * Updated revision to 5.0 and copyright to include 1994
 *
 * Revision 4.0  1993/06/24  17:14:56  sar
 * Updated rev to 4.0 and copyright to 93
 *
 * Revision 3.6  1993/05/13  19:08:06  sar
 * Added code to use a file implementation of nv memory
 *
 * Revision 3.5  1993/03/03  20:03:22  dab
 * don't undef __(), might need it
 *
 * Revision 3.4  1993/03/02  18:18:59  dab
 * Added read_config_defaults()
 *
 * Revision 3.3  1993/02/19  19:29:35  dab
 * Added mib_filename.
 *
 * Revision 3.2  1993/02/17  21:20:58  sar
 * Extern definition for spoid, dpoid, cpoid, spoidlen, dpoidlen and cpoidlen
 * Used to hold parties to send the packet to.
 *
 * Revision 3.1  92/06/16  16:05:42  dab
 * Added batch mode
 * 
 * Revision 3.0  92/04/03  19:53:14  dab
 * Release 3.0
 * 
 * Revision 2.100  91/08/09  14:05:22  dab
 * Update version number.  This is just before the work to upgrade to
 * release 3.0.
 * 
 * Revision 1.1  91/07/11  18:13:37  romkey
 * Initial revision
 * 
 */


/* [clearcase]
modification history
-------------------
01d,24dec12,bbz  retry_count has ambiguity, and it's name should include
				 module name(WIND00394780)
01c,22oct12,rjq  implemented SNMP manager over DTLS SSH
01b,25jun12,swg  add a macro to avoid redefination
01a,20apr05,job  update copyright notices
*/

#ifndef SNMPCONF_HEAD
#define SNMPCONF_HEAD

#if !INSTALL_ATTACHE
#if INSTALL_INETV6
#define MAX_IP_ADDR_LEN 16
#else
#define MAX_IP_ADDR_LEN 4
#endif

enum ipaddr_type { IPNONE = 0, IPV4 = 4, IPV6 = 6 };

typedef bits8_t ipaddr_bits_t;

typedef struct ipaddr {
  enum ipaddr_type type;
  ipaddr_bits_t addr[MAX_IP_ADDR_LEN];
} ipaddr_t;
#endif

#define ETC_ENV_VARIABLE "ETC_SNMP_CONF"

/* All supported transport for snmptalk client */
typedef struct snmptalk_supported_transport_struct_s {
    void     *(*snmptalk_udp_transport) (const char *, int, int);
    void     *(*snmptalk_udpipv6_transport) (const char *, int, int);
    void     *(*snmptalk_tls_transport) (const char *, int, int);
    void     *(*snmptalk_dtls_transport) (const char *, int, int);
    void     *(*snmptalk_dtlsipv6_transport) (const char *, int, int);
    void     *(*snmptalk_ssh_transport) (const char *, int, int);
} snmptalk_supported_transport_struct;


/* macros of the transport mode the client will be used. */
#define SNMPTALK_USE_UDP_TRANSPORT    1
#define SNMPTALK_USE_SSH_TRANSPORT    2
#define SNMPTALK_USE_TLS_TRANSPORT    3
#define SNMPTALK_USE_DTLS_TRANSPORT   4

/* TLS transport params for snmptalk client */
typedef struct snmptalk_tls_conf_struct {
    char     *local_cert_file;
    char     *local_key_file;
    char     *their_cert_file;
    char     *their_fingerprint;
    char     *their_hostname;
    char     *trust_cert;
} snmptalk_tls_conf;

extern snmptalk_supported_transport_struct snmptalk_supported_transport;

/* define the transport mode the client will be used. */
extern int  snmptalk_transport_mode;
/* client uses local port, default is 0 */
extern int  snmptalk_local_port;

/* TLS transport params */
extern snmptalk_tls_conf   snmptalk_tls;
/* SSH transport params */
extern char       *snmptalk_ssh_username;
extern char       *snmptalk_ssh_password;
extern char       *snmptalk_ssh_private_key;


extern char *read_community;
extern char *write_community;
extern char *destination;
extern char *mib_filename;
extern ipaddr_t dest_addr;
#if INSTALL_INETV6
extern int v6_scope;
#endif
extern int snmptalk_dest_port;
extern int snmp_timeout;
extern int snmptalk_send_retry_count;
extern int batch_mode;
extern int packet_dump;
extern int SNMPTalk_NV_Read;
extern int SNMPTalk_SNMP_Version;

extern int SNMP_Parse_Opts        (int, char **, char *);
extern int SNMP_Config            (void);

/* defaults for version 3 stuff */
#if INSTALL_ENVOY_SNMP_VERSION_3
extern char       v3_tos_flags;
extern char      *v3_con_name;
extern bits8_t    v3_con_id[];
extern ALENGTH_T  v3_con_id_len;
extern char      *v3_sec_name;
extern bits8_t    v3_sec_id[];
extern ALENGTH_T  v3_sec_id_len;
#endif


#if INSTALL_ENVOY_SNMP_VERSION_USEC
extern OCTET_T *usec_agentid;
extern OCTET_T *usec_context;
extern OCTET_T usec_qos;
#endif
#endif
