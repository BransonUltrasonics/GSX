/* auth_unix.h - Protocol for UNIX style authentication parameters for RPC */

/* Copyright 1984-1992, 1993-2010 Wind River Systems, Inc. */
/*
 * Sun RPC is a product of Sun Microsystems, Inc. and is provided for
 * unrestricted use provided that this legend is included on all tape
 * media and as a part of the software program in whole or part.  Users
 * may copy or modify Sun RPC without charge, but are not authorized
 * to license or distribute it to anyone else except as part of a product or
 * program developed by the user.
 *
 * SUN RPC IS PROVIDED AS IS WITH NO WARRANTIES OF ANY KIND INCLUDING THE
 * WARRANTIES OF DESIGN, MERCHANTIBILITY AND FITNESS FOR A PARTICULAR
 * PURPOSE, OR ARISING FROM A COURSE OF DEALING, USAGE OR TRADE PRACTICE.
 *
 * Sun RPC is provided with no support and without any obligation on the
 * part of Sun Microsystems, Inc. to assist in its use, correction,
 * modification or enhancement.
 *
 * SUN MICROSYSTEMS, INC. SHALL HAVE NO LIABILITY WITH RESPECT TO THE
 * INFRINGEMENT OF COPYRIGHTS, TRADE SECRETS OR ANY PATENTS BY SUN RPC
 * OR ANY PART THEREOF.
 *
 * In no event will Sun Microsystems, Inc. be liable for any lost revenue
 * or profits or other special, indirect and consequential damages, even if
 * Sun has been advised of the possibility of such damages.
 *
 * Sun Microsystems, Inc.
 * 2550 Garcia Avenue
 * Mountain View, California  94043
 */
/*      @(#)auth_unix.h 1.1 86/02/03 SMI      */

/*
 * auth_unix.h, Protocol for UNIX style authentication parameters for RPC
 *
 * Copyright (C) 1984, Sun Microsystems, Inc.
 */

/*
modification history
--------------------
01h,15jan10,y_t Add LP64 support.
01g,22sep92,rrr  added support for c++
01f,26may92,rrr  the tree shuffle
01e,04oct91,rrr  passed through the ansification filter
		  -fixed #else and #endif
		  -changed copyright notice
01d,24oct90,shl  moved authunix_create and authunix_create_default prototypes
		 to auth.h. deleted redundant function declarations.
01c,05oct90,shl  added ANSI function prototypes.
                 added copyright notice.
01b,26oct89,hjb  upgraded to release 4.0
*/

#ifndef __INCauth_unixh
#define __INCauth_unixh

#ifdef __cplusplus
extern "C" {
#endif

/*
 * The system is very weak.  The client uses no encryption for  it
 * credentials and only sends null verifiers.  The server sends backs
 * null verifiers or optionally a verifier that suggests a new short hand
 * for the credentials.
 */

/* The machine name is part of a credential; it may not exceed 255 bytes */
#define MAX_MACHINE_NAME 255

/* gids compose part of a credential; there may not be more than 10 of them */
#define NGRPS 16			/* 4.0 */

/*
 * Unix style credentials.
 */
struct authunix_parms {
	u_int 	 aup_time;
	char	*aup_machname;
	int	 aup_uid;
	int	 aup_gid;
	u_int	 aup_len;
	int	*aup_gids;
};


/*
 * If a response verifier has flavor AUTH_SHORT,
 * then the body of the response verifier encapsulates the following structure;
 * again it is serialized in the obvious fashion.
 */
struct short_hand_verf {
	struct opaque_auth new_cred;
};

/* function declarations */

#if defined(__STDC__) || defined(__cplusplus)

extern	  bool_t       xdr_authunix_parms (XDR *xdrs, struct authunix_parms *p);

#else

extern	  bool_t       xdr_authunix_parms ();

#endif	/* __STDC__ */

#ifdef __cplusplus
}
#endif

#endif /* __INCauth_unixh */
