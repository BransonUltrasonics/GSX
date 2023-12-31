/* auth.h - authentication interface */

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
/*      @(#)auth.h 1.1 86/02/03 SMI      */

/*
* auth.h, Authentication interface.
*
* Copyright (C) 1984, Sun Microsystems, Inc.
*
* The data structures are completely opaque to the client.  The client
* is required to pass a AUTH * to routines that create rpc
* "sessions".
*/

/*
modification history
--------------------
01j,02feb10,y_t update for LP64
01j,16nov09,lyc  fix WIND101949 - one structure does not support c++.
01i,22sep92,rrr  added support for c++
01h,26may92,rrr  the tree shuffle
01g,26feb92,wmd  added #pragma align 1 to align properly data structures.
01f,23feb91,del  removed useless checking and typedef of u_int32 as u_long.
01e,24oct90,shl  deleted redundant function declarations.
01d,05oct90,shl  added ANSI function prototypes.
                 made #endif ANSI style.
                 added copyright notice.
01c,30apr90,gae  fixed definition of u_int32 -- dependent of compiler define.
01b,26oct89,hjb  upgraded to release 4.0
*/

#ifndef __INCauthh
#define __INCauthh

#ifdef __cplusplus
extern "C" {
#endif

#if ((CPU_FAMILY==I960) && (defined __GNUC__))
#pragma align 1                 /* tell gcc960 not to optimize alignments */
#endif  /* CPU_FAMILY==I960 */

#define MAX_AUTH_BYTES	400
#define MAXNETNAMELEN	255   /* maximum length of network user's name - 4.0 */


/*
 * Status returned from authentication check
 */
enum auth_stat {
	AUTH_OK=0,
	/*
	 * failed at remote end
	 */
	AUTH_BADCRED=1,			/* bogus credentials (seal broken) */
	AUTH_REJECTEDCRED=2,		/* client should begin new session */
	AUTH_BADVERF=3,			/* bogus verifier (seal broken) */
	AUTH_REJECTEDVERF=4,		/* verifier expired or was replayed */
	AUTH_TOOWEAK=5,			/* rejected due to security reasons */
	/*
	 * failed locally
	*/
	AUTH_INVALIDRESP=6,		/* bogus response verifier */
	AUTH_FAILED=7			/* some unknown reason */
};

union des_block {
	struct {
		u_int high;
		u_int low;
	} key;
	char c[8];
};

typedef union des_block des_block;


/*
 * Authentication info.  Opaque to client.
 */
struct opaque_auth {
	enum_t	oa_flavor;		/* flavor of auth */
	caddr_t	oa_base;		/* address of more auth stuff */
	u_int	oa_length;		/* not to exceed MAX_AUTH_BYTES */
};


/*
 * Auth handle, interface to client side authenticators.
 */
typedef struct {
	struct	opaque_auth	ah_cred;
	struct	opaque_auth	ah_verf;
	union	des_block	ah_key;
	struct auth_ops {
#ifdef  __cplusplus
		void	(*ah_nextverf)(...);
		int	(*ah_marshal)(...);	/* nextverf & serialize */
		int	(*ah_validate)(...);	/* validate varifier */
		int	(*ah_refresh)(...);	/* refresh credentials */
		void	(*ah_destroy)(...);	/* destroy this structure */
#else
		void	(*ah_nextverf)();
		int	(*ah_marshal)();	/* nextverf & serialize */
		int	(*ah_validate)();	/* validate varifier */
		int	(*ah_refresh)();	/* refresh credentials */
		void	(*ah_destroy)();	/* destroy this structure */
#endif
	} *ah_ops;
	caddr_t ah_private;
} AUTH;


/*
 * Authentication ops.
 * The ops and the auth handle provide the interface to the authenticators.
 *
 * AUTH	*auth;
 * XDR	*xdrs;
 * struct opaque_auth verf;
 */
#define AUTH_NEXTVERF(auth)		\
		((*((auth)->ah_ops->ah_nextverf))(auth))
#define auth_nextverf(auth)		\
		((*((auth)->ah_ops->ah_nextverf))(auth))

#define AUTH_MARSHALL(auth, xdrs)	\
		((*((auth)->ah_ops->ah_marshal))(auth, xdrs))
#define auth_marshall(auth, xdrs)	\
		((*((auth)->ah_ops->ah_marshal))(auth, xdrs))

#define AUTH_VALIDATE(auth, verfp)	\
		((*((auth)->ah_ops->ah_validate))((auth), verfp))
#define auth_validate(auth, verfp)	\
		((*((auth)->ah_ops->ah_validate))((auth), verfp))

#define AUTH_REFRESH(auth)		\
		((*((auth)->ah_ops->ah_refresh))(auth))
#define auth_refresh(auth)		\
		((*((auth)->ah_ops->ah_refresh))(auth))

#define AUTH_DESTROY(auth)		\
		((*((auth)->ah_ops->ah_destroy))(auth))
#define auth_destroy(auth)		\
		((*((auth)->ah_ops->ah_destroy))(auth))


extern struct opaque_auth _null_auth;


/*
 * These are the various implementations of client side authenticators.
 */

/*
 * Unix style authentication
 * AUTH *authunix_create(machname, uid, gid, len, aup_gids)
 *	char *machname;
 *	int uid;
 *	int gid;
 *	int len;
 *	int *aup_gids;
 */
extern AUTH *authdes_create ();		/* 4.0 */

#define AUTH_NONE	0		/* no authentication -- 4.0 */
#define	AUTH_NULL	0		/* backward compatibility */
#define	AUTH_UNIX	1		/* unix style (uid, gids) */
#define	AUTH_SHORT	2		/* short hand unix style */
/* function declarations */

#if defined(__STDC__) || defined(__cplusplus)

extern	  AUTH *	authnone_create (void);
extern    AUTH *	authunix_create (char *machname, int uid, int gid,
					 int len, int *aup_gids);
extern 	  AUTH *	authunix_create_default(void); /* takes no parameters */
extern	  bool_t	xdr_des_block (XDR *xdrs, union des_block *blkp);

#else

extern    AUTH *	authunix_create ();
extern	  AUTH *	authnone_create ();
extern 	  AUTH *	authunix_create_default();    /* takes no parameters */
extern	  bool_t	xdr_des_block ();

#endif	/* __STDC__ */

#if ((CPU_FAMILY==I960) && (defined __GNUC__))
#pragma align 0                 /* turn off alignment requirement */
#endif  /* CPU_FAMILY==I960 */

#ifdef __cplusplus
}
#endif

#endif /* __INCauthh */
