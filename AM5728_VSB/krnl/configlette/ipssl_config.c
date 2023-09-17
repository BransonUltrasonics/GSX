/*
 * Copyright (c) 2006-2014, 2016 Wind River Systems, Inc.
 *
 * The right to copy, distribute, modify or otherwise make use
 * of this software may be licensed only pursuant to the terms
 * of an applicable Wind River license agreement.
 */

 
/*
modification history
--------------------
09dec16,chm  Eliminate the dependency between INCLUDE_IPFREESCALE and 
             INCLUDE_SHELL (F8092)
25aug14,ljl  Complete openssl layer and cleanup crypto/ssl, US44321
*/

/*
 ****************************************************************************
 * 1                    DESCRIPTION
 ****************************************************************************
 */

/*
 ****************************************************************************
 * 2                    INCLUDE FILES
 ****************************************************************************
 */
#include "ipcrypto_util.h"
#include "ipcrypto_shell.h"
#include <vsbConfig.h>

/*
 ****************************************************************************
 * 3                    DEFINES
 ****************************************************************************
 */
#define IPSSL_USE_TEST_CMDS
#define IPSSL_USE_CMDS

#ifdef PRJ_BUILD

#ifdef IPSSL_USE_TEST_CMDS
#undef IPSSL_USE_TEST_CMDS
#endif
#ifdef INCLUDE_IPSSL_USE_TEST_CMDS
#define IPSSL_USE_TEST_CMDS
#endif
#ifdef INCLUDE_IPSSL_USE_CMDS
#define IPSSL_USE_CMDS
#endif

#endif /* PRJ_BUILD */

#if defined(IPSSL_USE_CMDS) && defined(INCLUDE_IPSSL_USE_CMDS)
#define IPSSL_USE_CIPHERS_CMD
#define IPSSL_USE_S_CLIENT_CMD
#define IPSSL_USE_S_SERVER_CMD
#define IPSSL_USE_S_TIME_CMD
#ifdef INCLUDE_IPNET
#define IPSSL_USE_SSL_SRV_CMD
#define IPSSL_USE_SSL_CLT_CMD
#endif /*INCLUDE_IPNET*/
#endif /* IPSSL_USE_CMDS */

extern int ipssl_init(void);
/*
 ****************************************************************************
 * 6                    LOCAL PROTOTYPES
 ****************************************************************************
 */
#ifdef IPSSL_USE_CMDS
 void ipcrypto_cmds_startup(void);

#ifndef OPENSSL_NO_STDIO 
#ifdef IPSSL_USE_CIPHERS_CMD
 int ciphers_main(int argc, char** argv);
#endif
#ifdef IPSSL_USE_S_CLIENT_CMD
 int s_client_main(int argc, char** argv);
#endif
#ifdef IPSSL_USE_S_SERVER_CMD
 int s_server_main(int argc, char** argv);
#endif
#ifdef IPSSL_USE_S_TIME_CMD
 int s_time_main(int argc, char** argv);
#endif
#ifdef IPSSL_USE_SSL_CLT_CMD
 int ipssl_cmd_ssl_clt(int argc, char** argv);
#endif
#ifdef IPSSL_USE_SSL_SRV_CMD
 int ipssl_cmd_ssl_srv(int argc, char** argv);
#endif
#ifdef IPSSL_USE_TEST_CMDS
 int ssltest_main(int argc, char** argv);
#endif

#endif /*OPENSSL_NO_STDIO*/
#endif /*IPSSL_USE_CMDS*/



/*
 ****************************************************************************
 * 7                    DATA
 ****************************************************************************
 */

/*
 ****************************************************************************
 * 8                    STATIC FUNCTIONS
 ****************************************************************************
 */

/*
 ****************************************************************************
 * 9                   GLOBAL FUNCTIONS
 ****************************************************************************
 */
 
#if defined(IPSSL_USE_CIPHERS_CMD) && !defined(OPENSSL_NO_STDIO) 
	 int
	ipssl_ciphers(int argc, char** argv)
	{
		ipcrypto_cmds_startup();
		return ciphers_main(argc, argv);
	}
#endif
	
#if defined(IPSSL_USE_S_TIME_CMD) && !defined(OPENSSL_NO_STDIO) 
	 int
	ipssl_s_time(int argc, char** argv)
	{
		ipcrypto_cmds_startup();
		return s_time_main(argc, argv);
	}
#endif
	
#if defined(IPSSL_USE_S_CLIENT_CMD) && !defined(OPENSSL_NO_STDIO) 
	 int
	ipssl_s_client(int argc, char** argv)
	{
		ipcrypto_cmds_startup();
		return s_client_main(argc, argv);
	}
#endif
	
#if defined(IPSSL_USE_S_SERVER_CMD) && !defined(OPENSSL_NO_STDIO) 
	 int
	ipssl_s_server(int argc, char** argv)
	{
		ipcrypto_cmds_startup();
		return s_server_main(argc, argv);
	}
#endif

/*
 ****************************************************************************
 * 10                   PUBLIC FUNCTIONS
 ****************************************************************************
 */

/*
 *===========================================================================
 *                    ipssl_configure
 *===========================================================================
 */
int
ipssl_configure(void)
{
#ifndef OPENSSL_NO_STDIO 
#ifdef IPSSL_USE_CIPHERS_CMD
    (void)ipcrypto_shell_add_cmd("ciphers", "ciphers", "SSL Cipher Suites", ipssl_ciphers);
#endif

#ifdef IPSSL_USE_S_CLIENT_CMD
    (void)ipcrypto_shell_add_cmd("s_client", "s_client", "SSL client", ipssl_s_client);
#endif

#ifdef IPSSL_USE_S_SERVER_CMD
    (void)ipcrypto_shell_add_cmd("s_server", "s_server", "SSL server", ipssl_s_server);
#endif

#ifdef IPSSL_USE_S_TIME_CMD
    (void)ipcrypto_shell_add_cmd("s_time", "s_time", "Time SSL connection", ipssl_s_time);
#endif
#ifdef _WRS_CONFIG_IPNET_COREIP
#ifdef IPSSL_USE_SSL_CLT_CMD
    (void)ipcrypto_shell_add_cmd("ssl_clt", "ssl_clt", "SSL client for performance measurements ", ipssl_cmd_ssl_clt);
#endif

#ifdef IPSSL_USE_SSL_SRV_CMD
    (void)ipcrypto_shell_add_cmd("ssl_srv", "ssl_srv", "SSL server for performance measurements ", ipssl_cmd_ssl_srv);
#endif
#endif /*_WRS_CONFIG_IPNET_COREIP*/
#ifdef IPSSL_USE_TEST_CMDS
    (void)ipcrypto_shell_add_cmd("ssltest", "ssltest", "ssltest", ssltest_main);
#endif
#endif
    return 0;
}


/*
 ****************************************************************************
 *                      END OF FILE
 ****************************************************************************
 */

