/* 01comp_sec_key_db.cdf - Secure Key DB configuration file */

/*
 * Copyright (c) 2006-2009, 2014, 2016 Wind River Systems, Inc.
 *
 * The right to copy, distribute or otherwise make use of this software
 * may be licensed only pursuant to the terms of an applicable Wind River
 * license agreement. No license to Wind River intellectual property rights
 * is granted herein. All rights not licensed by Wind River are reserved
 * by Wind River.
 */

/*
modification history
--------------------
10mar16,m_c  Removed INCLUDE_IPCRYPTO_USE_KEY_DB_EXAMPLE_KEYS as it is a security issue
             Moved KEY_DATABASE from FOLDER_OPENSSL to FOLDER_VAULT
04mar16,m_c  Fixed name of KEY_DATABASE
10sep14,ljl  Split Key DB from ipcrypto, US44321
*/

Folder KEY_DATABASE {
   NAME            IPCOM key database
   _CHILDREN       FOLDER_VAULT
   CHILDREN        INCLUDE_IPCOM_USE_KEY_DB \
                   INCLUDE_IPCOM_KEY_DB_CMD
}

InitGroup usrKeydbCmd {
	  SYNOPSIS	Initialize the key store command
	  INIT_RTN   	usrKeydbCmdInitGrp ();
	  INIT_ORDER 	INCLUDE_IPCOM_KEY_DB_CMD
	  _INIT_ORDER	usrRoot
	  INIT_AFTER	usrNetworkInit
}
Component INCLUDE_IPCOM_USE_KEY_DB {
    NAME            Core module
    SYNOPSIS        The IPCOM key database stores keys in encrypted format. \
    		    Please note that the IPCOM_KEY_DB_KEY_ENCRYPTING_KEY parameter \
		    must be explicitly set.
    MODULES         ipcom_key_db.o
}

Component INCLUDE_IPCOM_KEY_DB_CMD {
    NAME            Shell command
    SYNOPSIS        Shell command to manipulate the IPCOM key database.
    REQUIRES	    INCLUDE_IPCOM_USE_KEY_DB
    PROTOTYPE	    void ipcom_cmd_keydb_init(void);
    INIT_RTN	    ipcom_cmd_keydb_init();
    _INIT_ORDER	    usrNetworkInit
    INIT_AFTER	    INCLUDE_IPCOM
}

