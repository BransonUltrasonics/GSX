/* 02ssi_snmp.cdf - SSI config file for SNMP component */

/* Copyright 2006-2012, 2014 Wind River Systems, Inc. 
 *  All rights reserved.  Provided under license only.
 *  Distribution or other use of this software is only
 *  permitted pursuant to the terms of a license agreement
 *  from Wind River Systems (and is otherwise prohibited).
 *  Refer to that license agreement for terms of use.
 */

/*
modification history
--------------------
30may14,bbz  resolve snmp engine's dependence on snmp agent(V7MAN-82)
14dec12,bbz  added REQUIRES for INCLUDE_SNMP_STATIC_INIT (WIND00328708)
22oct12,rjq  implemented SNMP manager over DTLS SSH
20jul12,rjq  implement RFC5590-5593, add TSM secmodel and
             TLS,DTLS,SSH transport model support.
18jan12,zhw added REQUIRES for INCLUDE_SNMP_STATIC_INIT (WIND00328708)
31dec11,zhw enable dynamic initialization for 32bit while disable it
            for 64bit since SSI has not supported 64bit (WIND00325865)
09sep10,y_t temporarily disable snmp dynamic initialization for 64bit
16aug07,mze rename INCLUDE_SNMPD to FOLDER_SNMPD
21aug07,tkf Remove INIT_RTN with non-existent routine. Change Selection
            name to adhere to standard coding convention (CQ:WIND000102067)
10may06,tkf UN_COMP component has to be automatically included when SNMP
            Dynamic initialization is included (WIND00044525).
03apr06,tkf Add the INCLUDE_WM_SNMP_INIT component as INCLUDE_DOT11_MIB
            depends on it (SPR#119209).
10mar06,xli added INCLUDE_DSI_DYNAMIC_INIT as the 
            INCLUDE_SNMP_DYNAMIC_INIT's REQUIRES 
04mar06,xli correct the cdf syntax error
30jan06.xli use selection clause
19jan06,xli Added INIT_RTN clause
04jan06,tkf  written
*/

/* 
DESCRIPTION

This file contains the SSI configuration file for the SNMP component. The
purpose of the file is to allow the SSI system co-exist with the legacy
VxWorks initialization system.

*/

Selection SELECT_SNMP_INIT {
        NAME            SNMP Initialization
        SYNOPSIS        Initialize SNMP component
        COUNT           1-1
        /*CHILDREN        INCLUDE_SNMP_DYNAMIC_INIT \
                        INCLUDE_SNMP_STATIC_INIT*/
        CHILDREN        INCLUDE_SNMP_STATIC_INIT
         _CHILDREN      FOLDER_SNMPD
        DEFAULTS        INCLUDE_SNMP_STATIC_INIT

}

/*
Component INCLUDE_SNMP_DYNAMIC_INIT {
	NAME	     SNMP Dynamic Initialization 
	SYNOPSIS     Wait for the SNMP component to be in the ready state
	HDR_FILES    ssi/ssm.h
        CFG_PARAMS   SSI_SNMP_NAME \
                     SSI_SNMP_DEPENDON \
                     SSI_SNMP_INIT_ROUTINE \
                     SSI_SNMP_ARGS \
                     SSI_SNMP_OPTIONS
        INIT_RTN     ssmCompWait (SSI_SNMP_NAME);
        REQUIRES     INCLUDE_SSI INCLUDE_DSI_DYNAMIC_INIT INCLUDE_UN_COMP INCLUDE_SNMP_UDP_TRANSPORT
}
*/

Component INCLUDE_SNMP_STATIC_INIT {
    NAME	SNMP Static Initialization
    SYNOPSIS	Initialize Wind Manage SNMP.
    CONFIGLETTES    usrSnmp.c
    INIT_RTN	usrSnmpCfgInit();
    REQUIRES    INCLUDE_SNMP_UDP_TRANSPORT
}
Component INCLUDE_SNMP_UDP_TRANSPORT {
	NAME	     SNMP udp transport model 
	SYNOPSIS     Enable SNMP udp transport model
	REQUIRES     INCLUDE_WINDMANAGE_SNMP_CORE
    _CHILDREN    FOLDER_SNMPD
}

#if defined(_WRS_CONFIG_SNMP_USE_SSH_TRANSPORT)
Component INCLUDE_SNMP_SSH_TRANSPORT {
	NAME	     SNMP ssh transport model 
	SYNOPSIS     Enable SNMP ssh transport model
	REQUIRES     INCLUDE_SSH \
	             INCLUDE_WINDMANAGE_SNMP_V3CORE
    _CHILDREN    FOLDER_SNMPD
}
#endif

#if defined(_WRS_CONFIG_SNMP_USE_TLS_DTLS_TRANSPORT)
Component INCLUDE_SNMP_TLS_TRANSPORT {
	NAME	     SNMP tls transport model 
	SYNOPSIS     Enable SNMP tls transport model
	REQUIRES     INCLUDE_SNMP_TLS_COMMON
    _CHILDREN    FOLDER_SNMPD
}

Component INCLUDE_SNMP_DTLS_TRANSPORT {
	NAME	     SNMP dtls transport model 
	SYNOPSIS     Enable SNMP dtls transport model
	REQUIRES     INCLUDE_SNMP_TLS_COMMON
    _CHILDREN    FOLDER_SNMPD
}

Component INCLUDE_SNMP_TLS_COMMON {
	NAME	     SNMP tls or dtls common configurations for SNMPD
	SYNOPSIS     SNMP tls or dtls common configurations for SNMPD. Ignored by snmp manager.
	CFG_PARAMS   SNMP_TLS_KEY_FILE \
	             SNMP_TLS_CERT_FILE \
	             SNMP_TLS_CA_CERT_FILE \
	             SNMP_TLS_CERT_SECNAME
	REQUIRES     INCLUDE_IPSSL \
	             INCLUDE_WINDMANAGE_SNMP_V3CORE
    _CHILDREN    FOLDER_SNMPD
}

#if defined(_WRS_CONFIG_SNMP_USE_SEC_KEY_DB)
Component INCLUDE_SNMP_PRIVACY {
    NAME            SNMP key Privacy support
    SYNOPSIS        This will use Secure Key Database to retrieve password for \
					SNMP private key for TLS or DTLS. Password and private key \
					have to be manually imported into secure key \
					database if this option has been selected.
    REQUIRES        INCLUDE_IPCOM_USE_KEY_DB
    _CHILDREN   	FOLDER_SNMPD
}

/*
Component INCLUDE_SNMP_TLS_COMMON {
    REQUIRES     += INCLUDE_SNMP_PRIVACY
}
*/
#endif
#endif


Component INCLUDE_WM_SNMP_INIT {
    NAME	        SNMP Static Initialization
    SYNOPSIS	    Initialize Wind Manage SNMP.
    /*CONFIGLETTES    ssi/usrSsi.c*/
    REQUIRES        SELECT_SNMP_INIT
    INCLUDE_WHEN    INCLUDE_WINDMANAGE_SNMP_CORE
}

Component INCLUDE_SNMP_MANAGER {
    NAME            SNMP manager
    SYNOPSIS        SNMP manager is used as a Network Management Station.
    CONFIGLETTES    usrSnmp.c
    INIT_RTN        usrSnmptalkInit();
    REQUIRES        INCLUDE_SNMP_UDP_TRANSPORT
    _CHILDREN       FOLDER_SNMPD
}

/*
Parameter SSI_SNMP_NAME {
        NAME		SNMP component name
	SYNOPSIS	SNMP component name
	TYPE		string
	DEFAULT		"snmp"
} 

Parameter SSI_SNMP_DEPENDON {
        NAME		SNMP depend on:
	SYNOPSIS	SNMP depend on list
	TYPE		string
	DEFAULT		"dsi"
}

Parameter SSI_SNMP_INIT_ROUTINE {
        NAME		SNMP init routine
	SYNOPSIS	SNMP init routine name
	TYPE		long
	DEFAULT		usrSsiSnmpInit
}

Parameter SSI_SNMP_ARGS {
        NAME		SNMP init arguments
	SYNOPSIS	SNMP init routine arguments
	TYPE		string
        DEFAULT         NULL
}

Parameter SSI_SNMP_OPTIONS {
        NAME		SNMP init options
	SYNOPSIS	SNMP component init options
	TYPE		string
        DEFAULT         NULL
}
*/

#if defined(_WRS_CONFIG_SNMP_USE_TLS_DTLS_TRANSPORT)
/******************** Advance Privacy Configuration Parameter   **********************/
/********* This will allowe to name the password ID or Key ID   **********************/
#if defined(_WRS_CONFIG_SNMP_USE_SEC_KEY_DB)
Parameter SNMP_TLS_KEY_ID {
    NAME        SNMP TLS key ID 
    SYNOPSIS    The key ID will be used to retrieve SSL/TLS key from secureKey Database. \
                The key ID should be imported into secure key db before SNMP DAEMON task runs. \
				E.g: "snmptlsPkeyId".
    TYPE        char *
    DEFAULT     NULL
    _CFG_PARAMS INCLUDE_SNMP_PRIVACY
}
#endif

Parameter SNMP_TLS_KEY_FILE {
    NAME        TLS key file
    SYNOPSIS    The path to the file containing the key pair to use in \
                SSL/TLS connections. Used by the implementation of \
                snmp_load_tls_key(). If INCLUDE_SNMP_PRIVACY is selected \
                and SNMP_TLS_KEY_ID is not NULL this field will be ignored. \
                User has to manually import this private key into secure key db before \
                SNMP DAEMON task runs.
    TYPE        char *
    DEFAULT     "/ram0/key"
    _CFG_PARAMS INCLUDE_SNMP_TLS_COMMON
}

Parameter SNMP_TLS_CERT_FILE {
    NAME        TLS certificate file
    SYNOPSIS    The path to the file containing the X509 certificate to \
                use in SSL/TLS connections. Used by the snmp_load_tls_cert ().
    TYPE        char *
    DEFAULT     "/ram0/cert.pem"
    _CFG_PARAMS INCLUDE_SNMP_TLS_COMMON
}

Parameter SNMP_TLS_CA_CERT_FILE {
    NAME        TLS CA certificate file
    SYNOPSIS    The path to the file containing the X509 certificate of \
                trusted CAs. SSL/TLS connections. Used by the snmp_load_ca_certs().
    TYPE        char *
    DEFAULT     "/ram0/CAcert.pem"
    _CFG_PARAMS INCLUDE_SNMP_TLS_COMMON
}

Parameter SNMP_TLS_CERT_SECNAME {
    NAME        configure a mapping for a SNMPv3 username
    SYNOPSIS    specify how to map a certificate field from the client's X.509 certificate \
                to a  SNMPv3 username. \
                The grammer is "certSecName PRIORITY FINGERPRINT OPTIONS". The certSecName is the keyword for \
                every certificate. The next parameter is priority. Then the FINGERPRINT is the fingerprint of \
                the certificate. The last parameter is the options. Use the --sn SECNAME flag \
                to directly specify a securityName for a given certificate.  The other flags \
                extract a particular component of the certificate for use as a snmpv3 securityName. \
                A SubjectAltName containing an rfc822 value (eg tester@windriver.com), A SubjectAltName \
                containing a dns name value (eg tester.windriver.com), an IP address (eg 192.0.2.1) or \
                a common name "windriver".   The --any flag specifies that  any  of  the  subjecAltName \
                fields may be used. Mutiple certificates should be separated with semicolon. \
                E.g: "certSecName 10 CA:B8:0A:B3:6B:4C:21:2A:F2:92:CD:0B:6B:DF:6A:9F:23:D6:30:4B --sn SnmpAdmin; \
                certSecName 20:56:E4:53:CE:D4:52:87:A7:74:11:BE:BA:9F:37:11:23:4A:77:CE:83 --cn"
    TYPE        char *
    DEFAULT     NULL
    _CFG_PARAMS INCLUDE_SNMP_TLS_COMMON
}
#endif
