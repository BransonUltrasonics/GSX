/* 00comp_vxKeyStore.cdf - Key Store components configuration file */

/*
 * Copyright (c) 2015-2017 Wind River Systems, Inc.
 *
 * The right to copy, distribute, modify, or otherwise make use
 * of this software may be licensed only pursuant to the terms
 * of an applicable Wind River license agreement.
 */


/*
modification history
--------------------
10jan17,h_x  Fix typo issue (V7SEC-345)
04mar16,m_c  Fixed name of components in FOLDER_VAULT
25may15,r_b  Created
*/

InitGroup usrVaultInit {
	  SYNOPSIS	Initialize the key store
	  INIT_RTN   	secVaultInitGrp ();
	  INIT_ORDER 	INCLUDE_SEC_VAULT \
	  		INCLUDE_SEC_VAULT_CMD \
			INCLUDE_SEC_KEY_STORE \
	  		INCLUDE_SEC_KEY_STORE_CMD \
	  		INCLUDE_SEC_VAULT_KEP_OBFUSCATOR
	  _INIT_ORDER	usrRoot
	  INIT_BEFORE   usrNetworkInit
	  INIT_AFTER	usrIosExtraInit
}

Folder FOLDER_VAULT {
       NAME	  Protected storage
       SYNOPSIS	  Storage protection for sensitive data.
       _CHILDREN  FOLDER_SECURITY
       CHILDREN	  INCLUDE_SEC_VAULT \
       		  INCLUDE_SEC_VAULT_CMD \
       		  INCLUDE_SEC_KEY_STORE \
       		  INCLUDE_SEC_KEY_STORE_CMD \
   		  SELECT_SEC_VAULT_KEP_BACKEND
}

Component INCLUDE_SEC_KEY_STORE {
        NAME            Key store module
        SYNOPSIS        Provides storage protection for openssl EVP_PKEY keys.
	MODULES		secKeyStoreLib.o
	PROTOTYPE	void secKeyStoreInit(void);
        INIT_RTN        secKeyStoreInit();
	REQUIRES	INCLUDE_SEC_VAULT
}

Component INCLUDE_SEC_KEY_STORE_CMD {
        NAME            Key store Shell command
        SYNOPSIS        Shell command for administration of OpenSSL EVP_PKEY keys in the key store.
	MODULES		secKeyStoreCmd.o
	PROTOTYPE	void secKeyStoreCmdInit(void);
        INIT_RTN        secKeyStoreCmdInit();
	REQUIRES	INCLUDE_SEC_KEY_STORE
}

Component INCLUDE_SEC_VAULT {
        NAME            Vault module
        SYNOPSIS        Provides storage protection for arbitrary secrets, \
			e.g. passwords, encryption keys, shared secrets, etc.
	MODULES		secVaultLib.o
	PROTOTYPE	void secVaultInit(const char*);
        INIT_RTN        secVaultInit(SEC_VAULT_FILE_ROOT);
	REQUIRES	SELECT_SEC_VAULT_KEP_BACKEND
	CFG_PARAMS	SEC_VAULT_FILE_ROOT
}

Component INCLUDE_SEC_VAULT_CMD {
        NAME            Vault Shell command
        SYNOPSIS        Shell command for administration of secrets 
	MODULES		secVaultCmd.o
	PROTOTYPE	void secVaultCmdInit(void);
        INIT_RTN        secVaultCmdInit();
	REQUIRES	INCLUDE_SEC_VAULT
}

Selection SELECT_SEC_VAULT_KEP_BACKEND {
        NAME            Key encryption Password (KEP) backend
        SYNOPSIS        Select the key encryption password backend for your project.
        COUNT           1-1
        CHILDREN        INCLUDE_SEC_VAULT_KEP_OBFUSCATOR
	DEFAULTS	INCLUDE_SEC_VAULT_KEP_OBFUSCATOR
}

Component INCLUDE_SEC_VAULT_KEP_OBFUSCATOR {
	NAME		Key encrypting password obfuscator
	SYNOPSIS	This component obfuscates the key-encrypting password and stores it in the image.
	PROTOTYPE	void secVaultKepObfuscatorInit(void);
	INIT_RTN	secVaultKepObfuscatorInit();
	CFG_PARAMS      SEC_VAULT_KEY_ENCRYPTING_PW
	CONFIGLETTES	secVaultKepObfuscatorCfg.c
}


Parameter SEC_VAULT_FILE_ROOT {
    NAME        Home directory
    SYNOPSIS    Top directory where the VxWorks vault directory structure shall be created. \
				For backwards compatibility reasons the default \
				value is set to /ram. In deployed systems the key db file root would \
				be set to a directory in a persistent file system, for example, "/flash".
    TYPE        char *
    DEFAULT	"/ram"
}

Parameter SEC_VAULT_KEY_ENCRYPTING_PW {
    NAME        Key encrypting password
    SYNOPSIS    Password used to encrypt keys in the vault and key store, when \
			INCLUDE_SEC_VAULT_KEP_OBFUSCATOR is used as the KEP backend. \
			This parameter should be set to a random string of at least 40 characters, \
			containing both lower and upper case, numbers and special characters. \
			SEC_VAULT_KEY_ENCRYPTING_PW has no default value on purpose. This \
			will force each customer to set a unique value.
    TYPE        char *
}
