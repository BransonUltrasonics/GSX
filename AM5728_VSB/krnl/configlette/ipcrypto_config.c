/*
 * Copyright (c) 2006-2014, 2016, 2017 Wind River Systems, Inc.
 *
 * The right to copy, distribute, modify or otherwise make use
 * of this software may be licensed only pursuant to the terms
 * of an applicable Wind River license agreement.
 */

/*
modification history
--------------------
10feb17,jxy  Remove IP_PUBLIC due to move the layer to the folder of
             security
09dec16,chm  Eliminate the dependency between INCLUDE_IPFREESCALE and 
             INCLUDE_SHELL (F8092)
09dec16,rbo  Added ipcrypto_get_root_stack_size
25aug14,ljl  Complete openssl layer and cleanup crypto/ssl, US44321
03apr14,jsh  deleted aes_ccm/aes_gcm test for upgrading to
             openssl-1_0_1
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
#include "ipcrypto_config.h"

/*
 ****************************************************************************
 * 3                    DEFINES
 ****************************************************************************
 */


#ifdef PRJ_BUILD

#ifdef IPCRYPTO_USE_TEST
#undef IPCRYPTO_USE_TEST
#endif
#ifdef INCLUDE_IPCRYPTO_USE_TEST_CMDS
#define IPCRYPTO_USE_TEST
#endif
#endif /* PRJ_BUILD */

/*
 *===========================================================================
 *                         IPCRYPTO_USE_*_CMD
 *===========================================================================
 * Set the defines below to include the correspondig  openssl shell command
 * applications.
 */

#if defined(IPCRYPTO_USE_APPS) && !defined(PRJ_BUILD) || defined(INCLUDE_IPCRYPTO_USE_CMDS)
#define IPCRYPTO_USE_VERIFY_CMD
#define IPCRYPTO_USE_ASN1PARSE_CMD
#define IPCRYPTO_USE_REQ_CMD
#define IPCRYPTO_USE_DGST_CMD
#define IPCRYPTO_USE_DH_CMD
#define IPCRYPTO_USE_DHPARAM_CMD
#define IPCRYPTO_USE_GENDH_CMD
#define IPCRYPTO_USE_ENC_CMD
#define IPCRYPTO_USE_PASSWD_CMD
#define IPCRYPTO_USE_CA_CMD
#define IPCRYPTO_USE_CRL_CMD
#define IPCRYPTO_USE_RSA_CMD
#define IPCRYPTO_USE_RSAUTL_CMD
#define IPCRYPTO_USE_GENRSA_CMD
#define IPCRYPTO_USE_DSA_CMD
#define IPCRYPTO_USE_DSAPARAM_CMD
#define IPCRYPTO_USE_GENDSA_CMD
#define IPCRYPTO_USE_X509_CMD
#define IPCRYPTO_USE_PKCS7_CMD
#define IPCRYPTO_USE_CRL2P7_CMD
#define IPCRYPTO_USE_NSEQ_CMD
#define IPCRYPTO_USE_PKCS8_CMD
#define IPCRYPTO_USE_SPKAC_CMD
#define IPCRYPTO_USE_SMIME_CMD
#define IPCRYPTO_USE_RAND_CMD
#define IPCRYPTO_USE_VERSION_CMD
#define IPCRYPTO_USE_ENGINE_CMD
#define IPCRYPTO_USE_SPEED_CMD
#ifdef IPSSL
#define IPCRYPTO_USE_OCSP_CMD
#endif

#endif /* IPCRYPTO_USE_APPS */


/*
 ****************************************************************************
 * 4                    TYPES
 ****************************************************************************
 */


/*
 ****************************************************************************
 * 5                    LOCAL PROTOTYPES
 ****************************************************************************
 */


/*
 ****************************************************************************
 * 6                    EXTERN PROTOTYPES
 ****************************************************************************
 */

int ipcom_cmd_keydb(int argc, char** argv);

/*
 *===========================================================================
 *                         apps
 *===========================================================================
 */

extern void ipcrypto_cmds_startup(void);

extern int verify_main(int argc, char ** argv);
extern int asn1parse_main(int argc, char ** argv);
extern int req_main(int argc,char *argv[]);
extern int dgst_main(int argc,char *argv[]);
extern int dh_main(int argc,char *argv[]);
extern int dhparam_main(int argc,char *argv[]);
extern int enc_main(int argc,char *argv[]);
extern int passwd_main(int argc,char *argv[]);
extern int gendh_main(int argc,char *argv[]);
extern int ca_main(int argc,char *argv[]);
extern int crl_main(int argc,char *argv[]);
extern int rsa_main(int argc,char *argv[]);
extern int rsautl_main(int argc,char *argv[]);
extern int dsa_main(int argc,char *argv[]);
extern int dsaparam_main(int argc,char *argv[]);
extern int x509_main(int argc,char *argv[]);
extern int genrsa_main(int argc,char *argv[]);
extern int gendsa_main(int argc,char *argv[]);
extern int speed_main(int argc,char *argv[]);
extern int version_main(int argc,char *argv[]);
extern int pkcs7_main(int argc,char *argv[]);
extern int crl2pkcs7_main(int argc,char *argv[]);
extern int nseq_main(int argc,char *argv[]);
extern int pkcs12_main(int argc,char *argv[]);
extern int pkcs8_main(int argc,char *argv[]);
extern int spkac_main(int argc,char *argv[]);
extern int smime_main(int argc,char *argv[]);
extern int rand_main(int argc,char *argv[]);
extern int engine_main(int argc,char *argv[]);
extern int ocsp_main(int argc,char *argv[]);


extern int ipcrypto_cmd_cmp(int argc, char **argv);
extern int ipcrypto_cmd_memory(int argc, char **argv);
extern int ipcrypto_init(void);

/*
 *===========================================================================
 *                         test
 *===========================================================================
 */
extern int bftest(int argc, char **argv);
extern int bntest(int argc, char **argv);
extern int divtest(int argc, char **argv);
extern int exptest(int argc, char **argv);
extern int casttest(int argc, char **argv);
extern int destest(int argc, char **argv);
extern int dhtest(int argc, char **argv);
extern int dsatest(int argc, char **argv);
extern int ectest(int argc, char **argv);
extern int ecdhtest(int argc, char **argv);
extern int ecdsatest(int argc, char **argv);
extern int enginetest(int argc, char **argv);
extern int hmactest(int argc, char **argv);
extern int evp_test(int argc, char **argv);
extern int hmactest(int argc, char **argv);
extern int md2test(int argc, char **argv);
extern int md4test(int argc, char **argv);
extern int md5test(int argc, char **argv);
extern int mdc2test(int argc, char **argv);
extern int randtest(int argc, char **argv);
extern int rc2test(int argc, char **argv);
extern int rc4test(int argc, char **argv);
extern int rc5test(int argc, char **argv);
extern int rmdtest(int argc, char **argv);
extern int rsa_test(int argc, char **argv);
extern int sha1test(int argc, char **argv);
extern int shatest(int argc, char **argv);
extern int sha512test(int argc, char **argv);
extern int tabtest(int argc, char **argv);
extern int ipcrypto_cmd_aescmac_test(int argc, char **argv);
extern int ipcrypto_cmd_aeskeywrap_test(int argc, char **argv);

/*
 ****************************************************************************
 * 7                    LOCAL PROTOTYPES
 ****************************************************************************
 */

#ifndef OPENSSL_NO_STDIO
#ifdef IPCRYPTO_USE_VERIFY_CMD
static int ipcrypto_verify_main(int argc, char ** argv);
#endif

#ifdef IPCRYPTO_USE_ASN1PARSE_CMD
static int ipcrypto_asn1parse_main(int argc, char ** argv);
#endif

#ifdef IPCRYPTO_USE_REQ_CMD
static int ipcrypto_req_main(int argc,char *argv[]);
#endif

#ifdef IPCRYPTO_USE_DGST_CMD
static int ipcrypto_dgst_main(int argc,char *argv[]);
#endif

#ifdef IPCRYPTO_USE_DH_CMD
static int ipcrypto_dh_main(int argc,char *argv[]);
#endif

#ifdef IPCRYPTO_USE_DHPARAM_CMD
static int ipcrypto_dhparam_main(int argc,char *argv[]);
#endif

#ifdef IPCRYPTO_USE_ENC_CMD
static int ipcrypto_enc_main(int argc,char *argv[]);
#endif

#ifdef IPCRYPTO_USE_PASSWD_CMD
static int ipcrypto_passwd_main(int argc,char *argv[]);
#endif

#ifdef IPCRYPTO_USE_GENDH_CMD
static int ipcrypto_gendh_main(int argc,char *argv[]);
#endif

#ifdef IPCRYPTO_USE_CA_CMD
static int ipcrypto_ca_main(int argc,char *argv[]);
#endif

#ifdef IPCRYPTO_USE_CRL_CMD
static int ipcrypto_crl_main(int argc,char *argv[]);
#endif

#ifdef IPCRYPTO_USE_RSA_CMD
static int ipcrypto_rsa_main(int argc,char *argv[]);
#endif

#ifdef IPCRYPTO_USE_RSAUTL_CMD
static int ipcrypto_rsautl_main(int argc,char *argv[]);
#endif

#ifdef IPCRYPTO_USE_DSA_CMD
static int ipcrypto_dsa_main(int argc,char *argv[]);
#endif

#ifdef IPCRYPTO_USE_DSAPARAM_CMD
static int ipcrypto_dsaparam_main(int argc,char *argv[]);
#endif

#ifdef IPCRYPTO_USE_X509_CMD
static int ipcrypto_x509_main(int argc,char *argv[]);
#endif

#ifdef IPCRYPTO_USE_GENRSA_CMD
static int ipcrypto_genrsa_main(int argc,char *argv[]);
#endif

#ifdef IPCRYPTO_USE_GENDSA_CMD
static int ipcrypto_gendsa_main(int argc,char *argv[]);
#endif

#ifdef IPCRYPTO_USE_SPEED_CMD
static int ipcrypto_speed_main(int argc,char *argv[]);
#endif

#ifdef IPCRYPTO_USE_VERSION_CMD
static int ipcrypto_version_main(int argc,char *argv[]);
#endif

#ifdef IPCRYPTO_USE_PKCS7_CMD
static int ipcrypto_pkcs7_main(int argc,char *argv[]);
#endif

#ifdef IPCRYPTO_USE_CRL2PKCS7_CMD
static int ipcrypto_crl2pkcs7_main(int argc,char *argv[]);
#endif

#ifdef IPCRYPTO_USE_NSEQ_CMD
static int ipcrypto_nseq_main(int argc,char *argv[]);
#endif

#ifdef IPCRYPTO_USE_PKCS12_CMD
static int ipcrypto_pkcs12_main(int argc,char *argv[]);
#endif

#ifdef IPCRYPTO_USE_PKCS8_CMD
static int ipcrypto_pkcs8_main(int argc,char *argv[]);
#endif

#ifdef IPCRYPTO_USE_SPKAC_CMD
static int ipcrypto_spkac_main(int argc,char *argv[]);
#endif

#ifdef IPCRYPTO_USE_SMIME_CMD
static int ipcrypto_smime_main(int argc,char *argv[]);
#endif

#ifdef IPCRYPTO_USE_RAND_CMD
static int ipcrypto_rand_main(int argc,char *argv[]);
#endif

#ifdef IPCRYPTO_USE_ENGINE_CMD
static int ipcrypto_engine_main(int argc,char *argv[]);
#endif

#ifdef IPCRYPTO_USE_OCSP_CMD
static int ipcrypto_ocsp_main(int argc,char *argv[]);
#endif
#endif /* #ifndef OPENSSL_NO_STDIO */


int ipcrypto_configure(void);

/*
 ****************************************************************************
 * 7                    DATA
 ****************************************************************************
 */

/*
 *===========================================================================
 *                         SYSVAR CONFIGURATION
 *===========================================================================
 */


/*
 ****************************************************************************
 * 9                    STATIC FUNCTIONS
 ****************************************************************************
 */


/*
 *===========================================================================
 *                    SHELL COMMANDS
 *===========================================================================
 */

#ifndef OPENSSL_NO_STDIO
#ifdef IPCRYPTO_USE_VERIFY_CMD
static int
ipcrypto_verify_main(int argc, char ** argv)
{
    ipcrypto_cmds_startup();
    return verify_main(argc, argv);
}
#endif

#ifdef IPCRYPTO_USE_ASN1PARSE_CMD
static int
ipcrypto_asn1parse_main(int argc, char ** argv)
{
    ipcrypto_cmds_startup();
    return asn1parse_main(argc, argv);
}
#endif

#ifdef IPCRYPTO_USE_REQ_CMD
static int
ipcrypto_req_main(int argc,char *argv[])
{
    ipcrypto_cmds_startup();
    return req_main(argc, argv);
}
#endif

#ifdef IPCRYPTO_USE_DGST_CMD
static int
ipcrypto_dgst_main(int argc,char *argv[])
{
    ipcrypto_cmds_startup();
    return dgst_main(argc, argv);
}
#endif

#ifdef IPCRYPTO_USE_DH_CMD
static int
ipcrypto_dh_main(int argc,char *argv[])
{
    ipcrypto_cmds_startup();
    return dh_main(argc, argv);
}
#endif

#ifdef IPCRYPTO_USE_DHPARAM_CMD
static int
ipcrypto_dhparam_main(int argc,char *argv[])
{
    ipcrypto_cmds_startup();
    return dhparam_main(argc, argv);
}
#endif

#ifdef IPCRYPTO_USE_ENC_CMD
static int
ipcrypto_enc_main(int argc,char *argv[])
{
    ipcrypto_cmds_startup();
    return enc_main(argc, argv);
}
#endif

#ifdef IPCRYPTO_USE_PASSWD_CMD
static int
ipcrypto_passwd_main(int argc,char *argv[])
{
    ipcrypto_cmds_startup();
    return passwd_main(argc, argv);
}
#endif

#ifdef IPCRYPTO_USE_GENDH_CMD
static int
ipcrypto_gendh_main(int argc,char *argv[])
{
    ipcrypto_cmds_startup();
    return gendh_main(argc, argv);
}
#endif

#ifdef IPCRYPTO_USE_CA_CMD
static int
ipcrypto_ca_main(int argc,char *argv[])
{
    ipcrypto_cmds_startup();
    return ca_main(argc, argv);
}
#endif

#ifdef IPCRYPTO_USE_CRL_CMD
static int
ipcrypto_crl_main(int argc,char *argv[])
{
    ipcrypto_cmds_startup();
    return crl_main(argc, argv);
}
#endif

#ifdef IPCRYPTO_USE_RSA_CMD
static int
ipcrypto_rsa_main(int argc,char *argv[])
{
    ipcrypto_cmds_startup();
    return rsa_main(argc, argv);
}
#endif

#ifdef IPCRYPTO_USE_RSAUTL_CMD
static int
ipcrypto_rsautl_main(int argc,char *argv[])
{
    ipcrypto_cmds_startup();
    return rsautl_main(argc, argv);
}
#endif

#ifndef OPENSSL_NO_DSA
#ifdef IPCRYPTO_USE_DSA_CMD
static int
ipcrypto_dsa_main(int argc,char *argv[])
{
    ipcrypto_cmds_startup();
    return dsa_main(argc, argv);
}
#endif

#ifdef IPCRYPTO_USE_GENDSA_CMD
static int
ipcrypto_gendsa_main(int argc,char *argv[])
{
    ipcrypto_cmds_startup();
    return gendsa_main(argc, argv);
}
#endif

#ifdef IPCRYPTO_USE_DSAPARAM_CMD
static int
ipcrypto_dsaparam_main(int argc,char *argv[])
{
    ipcrypto_cmds_startup();
    return dsaparam_main(argc, argv);
}
#endif
#endif /* OPENSSL_NO_DSA */

#ifdef IPCRYPTO_USE_X509_CMD
static int
ipcrypto_x509_main(int argc,char *argv[])
{
    ipcrypto_cmds_startup();
    return x509_main(argc, argv);
}
#endif

#ifdef IPCRYPTO_USE_GENRSA_CMD
static int
ipcrypto_genrsa_main(int argc,char *argv[])
{
    ipcrypto_cmds_startup();
    return genrsa_main(argc, argv);
}
#endif

#ifdef IPCRYPTO_USE_SPEED_CMD
static int
ipcrypto_speed_main(int argc,char *argv[])
{
    ipcrypto_cmds_startup();
    return speed_main(argc, argv);
}
#endif

#ifdef IPCRYPTO_USE_VERSION_CMD
static int
ipcrypto_version_main(int argc,char *argv[])
{
    ipcrypto_cmds_startup();
    return version_main(argc, argv);
}
#endif

#ifdef IPCRYPTO_USE_PKCS7_CMD
static int
ipcrypto_pkcs7_main(int argc,char *argv[])
{
    ipcrypto_cmds_startup();
    return pkcs7_main(argc, argv);
}
#endif

#ifdef IPCRYPTO_USE_CRL2PKCS7_CMD
static int
ipcrypto_crl2pkcs7_main(int argc,char *argv[])
{
    ipcrypto_cmds_startup();
    return crl2pkcs7_main(argc, argv);
}
#endif

#ifdef IPCRYPTO_USE_NSEQ_CMD
static int
ipcrypto_nseq_main(int argc,char *argv[])
{
    ipcrypto_cmds_startup();
    return nseq_main(argc, argv);
}
#endif

#ifdef IPCRYPTO_USE_PKCS12_CMD
static int
ipcrypto_pkcs12_main(int argc,char *argv[])
{
    ipcrypto_cmds_startup();
    return pkcs12_main(argc, argv);
}
#endif

#ifdef IPCRYPTO_USE_PKCS8_CMD
static int
ipcrypto_pkcs8_main(int argc,char *argv[])
{
    ipcrypto_cmds_startup();
    return pkcs8_main(argc, argv);
}
#endif

#ifdef IPCRYPTO_USE_SPKAC_CMD
static int
ipcrypto_spkac_main(int argc,char *argv[])
{
    ipcrypto_cmds_startup();
    return spkac_main(argc, argv);
}
#endif

#ifdef IPCRYPTO_USE_SMIME_CMD
static int
ipcrypto_smime_main(int argc,char *argv[])
{
    ipcrypto_cmds_startup();
    return smime_main(argc, argv);
}
#endif

#ifdef IPCRYPTO_USE_RAND_CMD
static int
ipcrypto_rand_main(int argc,char *argv[])
{
    ipcrypto_cmds_startup();
    return rand_main(argc, argv);
}
#endif

#ifdef IPCRYPTO_USE_ENGINE_CMD
static int
ipcrypto_engine_main(int argc,char *argv[])
{
    ipcrypto_cmds_startup();
    return engine_main(argc, argv);
}
#endif

#ifdef IPCRYPTO_USE_OCSP_CMD
static int
ipcrypto_ocsp_main(int argc,char *argv[])
{
    ipcrypto_cmds_startup();
    return ocsp_main(argc, argv);
}
#endif
#endif /* #ifndef OPENSSL_NO_STDIO */

/*
 ****************************************************************************
 * 10                    GLOBAL FUNCTIONS
 ****************************************************************************
 */

/*
 ****************************************************************************
 * 11                   PUBLIC FUNCTIONS
 ****************************************************************************
 */

/*
 *===========================================================================
 *                    ipcrypto_configure
 *===========================================================================
 */
int
ipcrypto_configure(void)
{
	/* Initialize IPCRYPTO system variables. */
/*    (void)ipcom_sysvar_setv(ipcrypto_conf_sysvar, IPCOM_SYSVAR_FLAG_OVERWRITE);*/

#ifdef INCLUDE_IPCOM_KEY_DB_CMD
    (void)ipcrypto_shell_add_cmd("keydb", "keydb <command> [options]", "Key DB admin command",
                              ipcom_cmd_keydb);
#endif

#if defined(IPCRYPTO_USE_VERIFY_CMD) && !defined(OPENSSL_NO_STDIO)
    (void)ipcrypto_shell_add_cmd("verify",
                  "verify [-verbose] [-CApath path] [-CAfile file] [-purpose purpose] cert1 cert2 ...",
                  "X.509 Certificate Verification", ipcrypto_verify_main);
#endif
#if defined(IPCRYPTO_USE_ASN1PARSE_CMD) && !defined(OPENSSL_NO_STDIO)
    (void)ipcrypto_shell_add_cmd("asn1parse", "asn1parse [options]",
                  "Parse an ASN.1 sequence", ipcrypto_asn1parse_main);
#endif
#if defined(IPCRYPTO_USE_REQ_CMD) && !defined(OPENSSL_NO_STDIO)
    (void)ipcrypto_shell_add_cmd("req", "req [options]",
                  "X.509 Certificate Signing Request (CSR) Management", ipcrypto_req_main);
#endif
#if defined(IPCRYPTO_USE_DGST_CMD) && !defined(OPENSSL_NO_STDIO)
    (void)ipcrypto_shell_add_cmd("dgst", "dgst [options] [file ...]", "Message Digest Calculation", ipcrypto_dgst_main);
#endif

#ifndef OPENSSL_NO_DH
#if defined(IPCRYPTO_USE_DH_CMD) && !defined(OPENSSL_NO_STDIO)
    (void)ipcrypto_shell_add_cmd("dh", "dh [options]",
                  "Diffie-Hellman Parameter Management. Obsoleted by dhparam", ipcrypto_dh_main);
#endif
#if defined(IPCRYPTO_USE_DHPARAM_CMD) && !defined(OPENSSL_NO_STDIO)
    (void)ipcrypto_shell_add_cmd("dhparam", "dhparam [options] [numbits]",
                  "Generation and Management of Diffie-Hellman Parameters", ipcrypto_dhparam_main);
#endif
#if defined(IPCRYPTO_USE_GENDH_CMD) && !defined(OPENSSL_NO_STDIO)
    (void)ipcrypto_shell_add_cmd("gendh", "x", "Generation of Diffie-Hellman Parameters. Obsoleted by dhparam", ipcrypto_gendh_main);
#endif
#endif /* OPENSSL_NO_DH */

#if defined(IPCRYPTO_USE_ENC_CMD) && !defined(OPENSSL_NO_STDIO)
    (void)ipcrypto_shell_add_cmd("enc", "enc [options]", "Encoding with Ciphers", ipcrypto_enc_main);
#endif
#if defined(IPCRYPTO_USE_PASSWD_CMD) && !defined(OPENSSL_NO_STDIO)
    (void)ipcrypto_shell_add_cmd("passwd", "passwd [options] [passwords]", "Generation of hashed passwords", ipcrypto_passwd_main);
#endif
#if defined(IPCRYPTO_USE_CA_CMD) && !defined(OPENSSL_NO_STDIO)
    (void)ipcrypto_shell_add_cmd("ca", "ca args", "Certificate Authority (CA) Management", ipcrypto_ca_main);
#endif
#if defined(IPCRYPTO_USE_CRL_CMD) && !defined(OPENSSL_NO_STDIO)
    (void)ipcrypto_shell_add_cmd("crl", "crl args", "Certificate Revocation List (CRL) Management", ipcrypto_crl_main);
#endif

#ifndef OPENSSL_NO_RSA
#if defined(IPCRYPTO_USE_RSA_CMD) && !defined(OPENSSL_NO_STDIO)
    (void)ipcrypto_shell_add_cmd("rsa", "rsa [options]", "RSA Data Management", ipcrypto_rsa_main);
#endif
#if defined(IPCRYPTO_USE_RSAUTL_CMD) && !defined(OPENSSL_NO_STDIO)
    (void)ipcrypto_shell_add_cmd("rsautl", "rsautl [options]",
                  "RSA utility for signing, verification, encryption, and decryption", ipcrypto_rsautl_main);
#endif
#if defined(IPCRYPTO_USE_GENRSA_CMD) && !defined(OPENSSL_NO_STDIO)
    (void)ipcrypto_shell_add_cmd("genrsa", "genrsa [args] [numbits]", "Generation of RSA keys", ipcrypto_genrsa_main);
#endif
#endif /* OPENSSL_NO_RSA */

#ifndef OPENSSL_NO_DSA
#if defined(IPCRYPTO_USE_DSA_CMD) && !defined(OPENSSL_NO_STDIO)
    (void)ipcrypto_shell_add_cmd("dsa", "dsa [options]", "DSA key processing", ipcrypto_dsa_main);
#endif
#if defined(IPCRYPTO_USE_DSAPARAM_CMD) && !defined(OPENSSL_NO_STDIO)
    (void)ipcrypto_shell_add_cmd("dsaparam", "dsaparam [options]", "DSA parameter manipulation and generation", ipcrypto_dsaparam_main);
#endif
#if defined(IPCRYPTO_USE_GENDSA_CMD) && !defined(OPENSSL_NO_STDIO)
    (void)ipcrypto_shell_add_cmd("gendsa", "gendsa [args] dsaparam-file",
                  "Generation of DSA Parameters", ipcrypto_gendsa_main);
#endif
#endif /* OPENSSL_NO_DSA */

#if defined(IPCRYPTO_USE_X509_CMD) && !defined(OPENSSL_NO_STDIO)
    (void)ipcrypto_shell_add_cmd("x509", "x509 [options]", "Certificate display and signing utility", ipcrypto_x509_main);
#endif
#if defined(IPCRYPTO_USE_SPEED_CMD) && !defined(OPENSSL_NO_STDIO)
    (void)ipcrypto_shell_add_cmd("speed", "speed [options]", "Test encryption performance", ipcrypto_speed_main);
#endif

#if defined(IPCRYPTO_USE_PKCS7_CMD) && !defined(OPENSSL_NO_STDIO)
    (void)ipcrypto_shell_add_cmd("pkcs7", "pkcs7 [options]", "PKCS#7 Data Management", ipcrypto_pkcs7_main);
#endif
#if defined(IPCRYPTO_USE_CRL2PKCS7_CMD) && !defined(OPENSSL_NO_STDIO)
    (void)ipcrypto_shell_add_cmd("crl2p7", "crl2p7 [options]", "CRL to PKCS#7 Conversion", ipcrypto_crl2pkcs7_main);
#endif
#if defined(IPCRYPTO_USE_NSEQ_CMD) && !defined(OPENSSL_NO_STDIO)
    (void)ipcrypto_shell_add_cmd("nseq", "nseq [options]", "Create or examine a netscape certificate sequence", ipcrypto_nseq_main);
#endif
#if defined(IPCRYPTO_USE_PKCS12_CMD) &&!defined(OPENSSL_NO_DES) && !defined(OPENSSL_NO_SHA1) && !defined(OPENSSL_NO_STDIO)
    (void)ipcrypto_shell_add_cmd("pkcs12", "pkcs12 [options]", "PKCS#12 file utility", ipcrypto_pkcs12_main);
#endif
#if defined(IPCRYPTO_USE_PKCS8_CMD) && !defined(OPENSSL_NO_STDIO)
    (void)ipcrypto_shell_add_cmd("pkcs8", "pkcs8 [options]", "PKCS#8 format private key conversion tool", ipcrypto_pkcs8_main);
#endif
#if defined(IPCRYPTO_USE_SPKAC_CMD) && !defined(OPENSSL_NO_STDIO)
    (void)ipcrypto_shell_add_cmd("spkac", "spkac [options]", "SPKAC printing and generating utility", ipcrypto_spkac_main);
#endif
#if defined(IPCRYPTO_USE_SMIME_CMD) && !defined(OPENSSL_NO_STDIO)
    (void)ipcrypto_shell_add_cmd("smime", "smime [options]", "S/MIME mail processing", ipcrypto_smime_main);
#endif
#if defined(IPCRYPTO_USE_RAND_CMD) && !defined(OPENSSL_NO_STDIO)
    (void)ipcrypto_shell_add_cmd("rand", "rand [options]", "Generate pseudo-random bytes", ipcrypto_rand_main);
#endif

#if defined(IPCRYPTO_USE_VERSION_CMD) && !defined(OPENSSL_NO_STDIO)
    (void)ipcrypto_shell_add_cmd("ipcrypto_ver", "ipcrypto_ver", "Show IPCRYPTO version", ipcrypto_version_main);
#endif

#if defined(IPCRYPTO_USE_ENGINE_CMD) && !defined(OPENSSL_NO_STDIO)
    (void)ipcrypto_shell_add_cmd("engine", "engine", "Hardware Crypto Support", ipcrypto_engine_main);
#endif

#if defined(IPCRYPTO_USE_OCSP_CMD) && !defined(OPENSSL_NO_STDIO)
    (void)ipcrypto_shell_add_cmd("ocsp", "ocsp", "ocsp - Online Certificate Status Protocol utility", ipcrypto_ocsp_main);
#endif

#ifdef IPCRYPTO_USE_TEST
    (void)ipcrypto_shell_add_cmd("cmp",     "cmp",    "cmp",    ipcrypto_cmd_cmp);
    (void)ipcrypto_shell_add_cmd("sslmem",  "sslmem", "sslmem", ipcrypto_cmd_memory);

    /* general test/ progs. */
#ifndef OPENSSL_NO_STDIO
    (void)ipcrypto_shell_add_cmd("bntest", "bntest", "Big number test utility.", bntest);
    (void)ipcrypto_shell_add_cmd("exptest", "exptest", "Exponent test utility.", exptest);
#endif
#ifndef OPENSSL_NO_EC
    (void)ipcrypto_shell_add_cmd("ectest", "ectest", "ectest test utility.", ectest);
    (void)ipcrypto_shell_add_cmd("ecdhtest", "ecdhtest", "ecdhtest test utility.", ecdhtest);
    (void)ipcrypto_shell_add_cmd("ecdsatest", "ecdsatest", "ecdsatest test utility.", ecdsatest);
#endif
#if !defined(OPENSSL_NO_ENGINE) && !defined(OPENSSL_NO_STDIO)
    (void)ipcrypto_shell_add_cmd("enginetest", "enginetest", "engine test utility.", enginetest);
#endif
#ifndef OPENSSL_NO_MD2
    (void)ipcrypto_shell_add_cmd("md2test", "md2test", "md2test test utility.", md2test);
#endif
#ifndef OPENSSL_NO_MD4
    (void)ipcrypto_shell_add_cmd("md4test", "md4test", "md4test test utility.", md4test);
#endif
#ifndef OPENSSL_NO_MD5
    (void)ipcrypto_shell_add_cmd("md5test", "md5test", "md5test test utility.", md5test);
#endif
#ifndef OPENSSL_NO_HMAC
    (void)ipcrypto_shell_add_cmd("hmactest", "hmactest", "hmactest test utility.", hmactest);
#endif
#ifndef OPENSSL_NO_RC2
    (void)ipcrypto_shell_add_cmd("rc2test", "rc2test", "RC2 test utility.", rc2test);
#endif
#ifndef OPENSSL_NO_RC4
    (void)ipcrypto_shell_add_cmd("rc4test", "rc4test", "RC4 test utility.", rc4test);
#endif
#ifndef OPENSSL_NO_DES
    (void)ipcrypto_shell_add_cmd("destest", "destest", "destest test utility.", destest);
#endif
#ifndef OPENSSL_NO_SHA
#ifndef OPENSSL_FIPS
    (void)ipcrypto_shell_add_cmd("shatest", "shatest", "shatest test utility.", shatest);
    (void)ipcrypto_shell_add_cmd("sha512test", "sha512test", "sha512 test utility.", sha512test);
#endif
    (void)ipcrypto_shell_add_cmd("sha1test", "sha1test", "sha1test test utility.", sha1test);

#endif
#ifndef OPENSSL_NO_MDC2
    (void)ipcrypto_shell_add_cmd("mdc2test", "mdc2test", "mdc2test test utility.", mdc2test);
#endif
#ifndef OPENSSL_NO_RIPEMD
    (void)ipcrypto_shell_add_cmd("rmdtest", "rmdtest", "rmdtest test utility.", rmdtest);
#endif
    (void)ipcrypto_shell_add_cmd("randtest", "randtest", "randtest test utility.", randtest);
#if !defined(OPENSSL_NO_DH) && !defined(OPENSSL_NO_STDIO)
    (void)ipcrypto_shell_add_cmd("dhtest", "dhtest", "dhtest test utility.", dhtest);
#endif
#ifndef OPENSSL_NO_CAST
    (void)ipcrypto_shell_add_cmd("casttest", "casttest", "casttest test utility.", casttest);
#endif
#ifndef OPENSSL_NO_BF
    (void)ipcrypto_shell_add_cmd("bftest", "bftest", "bftest test utility.", bftest);
#endif
#if !defined(OPENSSL_NO_DSA) && !defined(OPENSSL_NO_STDIO)
    (void)ipcrypto_shell_add_cmd("dsatest", "dsatest", "dsatest test utility.", dsatest);
#endif
#ifndef OPENSSL_NO_RSA
    (void)ipcrypto_shell_add_cmd("rsa_test", "rsa_test", "rsa_test test utility.", rsa_test);
#endif
#if !defined(OPENSSL_NO_EVP) && !defined(OPENSSL_NO_STDIO)
    (void)ipcrypto_shell_add_cmd("evp_test", "evp_test", "evp_test test utility.", evp_test);
#endif

    (void)ipcrypto_shell_add_cmd("tabtest", "tabtest", "X509 test utility.", tabtest);
    (void)ipcrypto_shell_add_cmd("cmactest", "cmactest", "AES-CMAC test utility.",ipcrypto_cmd_aescmac_test);
    (void)ipcrypto_shell_add_cmd("aeskeywraptest", "aeskeywraptest", "AES-Keywrap test utility.",ipcrypto_cmd_aeskeywrap_test);

#endif /* IPCRYPTO_USE_TEST */

    /* No configuration to enter */
    return 0;
}

/*
 *===========================================================================
 *                    ipcrypto_get_root_stack_size
 *===========================================================================
 */
size_t ipcrypto_get_root_stack_size(void)
{
    return ROOT_STACK_SIZE;
}

/*
 ****************************************************************************
 *                      END OF FILE
 ****************************************************************************
 */
