/* This file was automatically generated from errno.h */
/* Run "make mkstrerr" to regenerate this file */
#include "rtld.h"
#include <errno.h>
#include <string.h>
int xsnprintf(char *buf, size_t buflen, const char *fmt, ...);
char *_rtld_xstrerror(int e) {
	static char buf[64];

	switch (e) {
#ifdef EPERM
	case EPERM:
		return "Not owner";
#endif
#ifdef ENOENT
	case ENOENT:
		return "No such file or directory";
#endif
#ifdef ESRCH
	case ESRCH:
		return "No such process";
#endif
#ifdef EINTR
	case EINTR:
		return "Interrupted system call";
#endif
#ifdef EIO
	case EIO:
		return "I/O error";
#endif
#ifdef ENXIO
	case ENXIO:
		return "No such device or address";
#endif
#ifdef E2BIG
	case E2BIG:
		return "Arg list too long";
#endif
#ifdef ENOEXEC
	case ENOEXEC:
		return "Exec format error";
#endif
#ifdef EBADF
	case EBADF:
		return "Bad file number";
#endif
#ifdef ECHILD
	case ECHILD:
		return "No children";
#endif
#ifdef EAGAIN
	case EAGAIN:
		return "No more processes";
#endif
#ifdef ENOMEM
	case ENOMEM:
		return "Not enough core";
#endif
#ifdef EACCES
	case EACCES:
		return "Permission denied";
#endif
#ifdef EFAULT
	case EFAULT:
		return "Bad address";
#endif
#ifdef ENOTEMPTY
	case ENOTEMPTY:
		return "Directory not empty";
#endif
#ifdef EBUSY
	case EBUSY:
		return "Mount device busy";
#endif
#ifdef EEXIST
	case EEXIST:
		return "File exists";
#endif
#ifdef EXDEV
	case EXDEV:
		return "Cross-device link";
#endif
#ifdef ENODEV
	case ENODEV:
		return "No such device";
#endif
#ifdef ENOTDIR
	case ENOTDIR:
		return "";
#endif
#ifdef EISDIR
	case EISDIR:
		return "Is a directory";
#endif
#ifdef EINVAL
	case EINVAL:
		return "Invalid argument";
#endif
#ifdef ENFILE
	case ENFILE:
		return "File table overflow";
#endif
#ifdef EMFILE
	case EMFILE:
		return "Too many open files";
#endif
#ifdef ENOTTY
	case ENOTTY:
		return "Not a typewriter";
#endif
#ifdef ENAMETOOLONG
	case ENAMETOOLONG:
		return "File name too long";
#endif
#ifdef EFBIG
	case EFBIG:
		return "File too large";
#endif
#ifdef ENOSPC
	case ENOSPC:
		return "No space left on device";
#endif
#ifdef ESPIPE
	case ESPIPE:
		return "Illegal seek";
#endif
#ifdef EROFS
	case EROFS:
		return "Read-only file system";
#endif
#ifdef EMLINK
	case EMLINK:
		return "Too many links";
#endif
#ifdef EPIPE
	case EPIPE:
		return "Broken pipe";
#endif
#ifdef EDEADLK
	case EDEADLK:
		return "Resource deadlock avoided";
#endif
#ifdef ENOLCK
	case ENOLCK:
		return "No locks available";
#endif
#ifdef ENOTSUP
	case ENOTSUP:
		return "Unsupported value";
#endif
#ifdef EMSGSIZE
	case EMSGSIZE:
		return "Message size";
#endif
#ifdef EDOM
	case EDOM:
		return "Argument too large";
#endif
#ifdef ERANGE
	case ERANGE:
		return "Result too large";
#endif
#ifdef EDESTADDRREQ
	case EDESTADDRREQ:
		return "Destination address required";
#endif
#ifdef EPROTOTYPE
	case EPROTOTYPE:
		return "Protocol wrong type for socket";
#endif
#ifdef ENOPROTOOPT
	case ENOPROTOOPT:
		return "Protocol not available";
#endif
#ifdef EPROTONOSUPPORT
	case EPROTONOSUPPORT:
		return "Protocol not supported";
#endif
#ifdef ESOCKTNOSUPPORT
	case ESOCKTNOSUPPORT:
		return "Socket type not supported";
#endif
#ifdef EOPNOTSUPP
	case EOPNOTSUPP:
		return "Operation not supported on socket";
#endif
#ifdef EPFNOSUPPORT
	case EPFNOSUPPORT:
		return "Protocol family not supported";
#endif
#ifdef EAFNOSUPPORT
	case EAFNOSUPPORT:
		return "Addr family not supported";
#endif
#ifdef EADDRINUSE
	case EADDRINUSE:
		return "Address already in use";
#endif
#ifdef EADDRNOTAVAIL
	case EADDRNOTAVAIL:
		return "Can't assign requested address";
#endif
#ifdef ENOTSOCK
	case ENOTSOCK:
		return "Socket operation on non-socket";
#endif
#ifdef ENETUNREACH
	case ENETUNREACH:
		return "Network is unreachable";
#endif
#ifdef ENETRESET
	case ENETRESET:
		return "";
#endif
#ifdef ECONNABORTED
	case ECONNABORTED:
		return "Software caused connection abort";
#endif
#ifdef ECONNRESET
	case ECONNRESET:
		return "Connection reset by peer";
#endif
#ifdef ENOBUFS
	case ENOBUFS:
		return "No buffer space available";
#endif
#ifdef EISCONN
	case EISCONN:
		return "Socket is already connected";
#endif
#ifdef ENOTCONN
	case ENOTCONN:
		return "Socket is not connected";
#endif
#ifdef ESHUTDOWN
	case ESHUTDOWN:
		return "Can't send after socket shutdown";
#endif
#ifdef ETOOMANYREFS
	case ETOOMANYREFS:
		return "Too many references: can't splice";
#endif
#ifdef ETIMEDOUT
	case ETIMEDOUT:
		return "Connection timed out";
#endif
#ifdef ECONNREFUSED
	case ECONNREFUSED:
		return "Connection refused";
#endif
#ifdef ENETDOWN
	case ENETDOWN:
		return "Network is down";
#endif
#ifdef ETXTBSY
	case ETXTBSY:
		return "Text file busy";
#endif
#ifdef ELOOP
	case ELOOP:
		return "Too many levels of symbolic links";
#endif
#ifdef EHOSTUNREACH
	case EHOSTUNREACH:
		return "No route to host";
#endif
#ifdef ENOTBLK
	case ENOTBLK:
		return "Block device required";
#endif
#ifdef EHOSTDOWN
	case EHOSTDOWN:
		return "Host is down";
#endif
#ifdef EINPROGRESS
	case EINPROGRESS:
		return "Operation now in progress";
#endif
#ifdef EALREADY
	case EALREADY:
		return "Operation already in progress";
#endif
#ifdef EWOULDBLOCK
	case EWOULDBLOCK:
		return "Operation would block";
#endif
#ifdef ENOSYS
	case ENOSYS:
		return "Function not implemented";
#endif
#ifdef ECANCELED
	case ECANCELED:
		return "Operation canceled";
#endif
#ifdef ENOSR
	case ENOSR:
		return "Insufficient memory";
#endif
#ifdef ENOSTR
	case ENOSTR:
		return "STREAMS device required";
#endif
#ifdef EPROTO
	case EPROTO:
		return "Generic STREAMS error";
#endif
#ifdef EBADMSG
	case EBADMSG:
		return "Invalid STREAMS message";
#endif
#ifdef ENODATA
	case ENODATA:
		return "Missing expected message data";
#endif
#ifdef ETIME
	case ETIME:
		return "STREAMS timeout occurred";
#endif
#ifdef ENOMSG
	case ENOMSG:
		return "Unexpected message type";
#endif
#ifdef EFPOS
	case EFPOS:
		return "File positioning error";
#endif
#ifdef EILSEQ
	case EILSEQ:
		return "Illegal byte sequence.";
#endif
#ifdef EDQUOT
	case EDQUOT:
		return "routing error";
#endif
#ifdef EIDRM
	case EIDRM:
		return "Identifier removed";
#endif
#ifdef EOVERFLOW
	case EOVERFLOW:
		return "Value too large for data type";
#endif
#ifdef EMULTIHOP
	case EMULTIHOP:
		return "Reserved";
#endif
#ifdef ENOLINK
	case ENOLINK:
		return "Reserved";
#endif
#ifdef ESTALE
	case ESTALE:
		return "Reserved";
#endif
	default:
		_rtld_xsnprintf(buf, sizeof (buf), "Error 0x%x", e);
		return buf;
	}
}
