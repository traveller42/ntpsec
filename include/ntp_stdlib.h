/*
 * ntp_stdlib.h - Prototypes for NTP lib.
 */
#ifndef GUARD_NTP_STDLIB_H
#define GUARD_NTP_STDLIB_H

#include <sys/types.h>
#include <sys/socket.h>
#include <string.h>
#include <errno.h>
#include <stdarg.h>

#include "declcond.h"	/* ntpd uses ntpd/declcond.h, others include/ */
#include "ntp_net.h"
#include "ntp_debug.h"
#include "ntp_malloc.h"
#include "ntp_syslog.h"

#ifdef __GNUC__
#define NTP_PRINTF(fmt, args) __attribute__((__format__(__printf__, fmt, args)))
#else
#define NTP_PRINTF(fmt, args)
#endif

extern const char *Version;
extern const char *VVersion;

extern	int	mprintf(const char *, ...) NTP_PRINTF(1, 2);
extern	int	mfprintf(FILE *, const char *, ...) NTP_PRINTF(2, 3);
extern	int	mvfprintf(FILE *, const char *, va_list) NTP_PRINTF(2, 0);
extern	int	mvsnprintf(char *, size_t, const char *, va_list)
			NTP_PRINTF(3, 0);
extern	int	msnprintf(char *, size_t, const char *, ...)
			NTP_PRINTF(3, 4);
extern	void	msyslog(int, const char *, ...) NTP_PRINTF(2, 3);
extern	void	init_logging	(const char *, uint32_t, int);
extern	int	change_logfile	(const char *, int);
extern	void	setup_logfile	(const char *);
extern	void	errno_to_str(int, char *, size_t);

/*
 * When building without OpenSSL, use a few macros of theirs to
 * minimize source differences in NTP.
 */
#ifndef HAVE_OPENSSL
#define NID_md5	4	/* from openssl/objects.h */
/* from openssl/evp.h */
#define EVP_MAX_MD_SIZE	64	/* longest known is SHA512 */
#endif

#define SAVE_ERRNO(stmt)				\
	{						\
		int preserved_errno;			\
							\
		preserved_errno = socket_errno();	\
		{					\
			stmt				\
		}					\
		errno = preserved_errno;		\
	}

typedef void (*ctrl_c_fn)(void);

/* authkeys.c */
extern	void	auth_delkeys	(void);
extern	int	auth_havekey	(keyid_t);
extern	int	authdecrypt	(keyid_t, uint32_t *, int, int);
extern	int	authencrypt	(keyid_t, uint32_t *, int);
extern	int	authhavekey	(keyid_t);
extern	int	authistrusted	(keyid_t);
extern	bool	authreadkeys	(const char *);
extern	void	authtrust	(keyid_t, u_long);
extern	bool	authusekey	(keyid_t, int, const uint8_t *);

/*
 * Based on the NTP timestamp, calculate the NTP timestamp of
 * the corresponding calendar unit. Use the pivot time to unfold
 * the NTP timestamp properly, or the current system time if the
 * pivot pointer is NULL.
 */
extern	uint32_t	calyearstart	(uint32_t ntptime, const time_t *pivot);
extern	uint32_t	calmonthstart	(uint32_t ntptime, const time_t *pivot);
extern	uint32_t	calweekstart	(uint32_t ntptime, const time_t *pivot);
extern	uint32_t	caldaystart	(uint32_t ntptime, const time_t *pivot);

extern	const char *clockname	(int);
extern	int	clocktime	(int, int, int, int, int, uint32_t, u_long *, uint32_t *);
extern	void	init_auth	(void);
extern	void	init_lib	(void);
extern	struct savekey *auth_findkey (keyid_t);
extern	void	auth_moremem	(int);
extern	void	auth_prealloc_symkeys(int);
extern	int	ymd2yd		(int, int, int);

/* getopt.c */
struct option {
    const char* name;
    int has_arg;
    int* flag;
    int val;
};

int ntp_getopt(int argc, char *const argv[], const char *optstring);

int ntp_getopt_long(int argc, char* const argv[], const char *optstring,
		    const struct option *longopts, int *longindex);

/* a_md5encrypt.c */
extern	int	MD5authdecrypt	(int, uint8_t *, uint32_t *, int, int);
extern	int	MD5authencrypt	(int, uint8_t *, uint32_t *, int);
extern	void	MD5auth_setkey	(keyid_t, int, const uint8_t *, size_t);
extern	uint32_t	addr2refid	(sockaddr_u *);

/* emalloc.c */
#ifndef EREALLOC_CALLSITE	/* ntp_malloc.h defines */
extern	void *	ereallocz	(void *, size_t, size_t, int);
extern	void *	oreallocarray	(void *optr, size_t nmemb, size_t size);
#define	erealloczsite(p, n, o, z, f, l) ereallocz((p), (n), (o), (z))
#define	emalloc(n)		ereallocz(NULL, (n), 0, false)
#define	emalloc_zero(c)		ereallocz(NULL, (c), 0, true)
#define	erealloc(p, c)		ereallocz((p), (c), 0, false)
#define erealloc_zero(p, n, o)	ereallocz((p), (n), (o), true)
#define ereallocarray(p, n, s)	oreallocarray((p), (n), (s))
#define eallocarray(n, s)	oreallocarray(NULL, (n), (s))
extern	char *	estrdup_impl(const char *);
#define	estrdup(s)		estrdup_impl(s)
#else
extern	void *	ereallocz	(void *, size_t, size_t, int,
				 const char *, int);
extern	void *	oreallocarray	(void *optr, size_t nmemb, size_t size,
				 const char *, int);
#define erealloczsite		ereallocz
#define	emalloc(c)		ereallocz(NULL, (c), 0, false, \
					  __FILE__, __LINE__)
#define	emalloc_zero(c)		ereallocz(NULL, (c), 0, true, \
					  __FILE__, __LINE__)
#define	erealloc(p, c)		ereallocz((p), (c), 0, false,	\
					  __FILE__, __LINE__)
#define	erealloc_zero(p, n, o)	ereallocz((p), n, (o), true,	\
					  __FILE__, __LINE__)
#define ereallocarray(p, n, s)	oreallocarray((p), (n), (s), \
 					  __FILE__, __LINE__)
#define eallocarray(n, s)	oreallocarray(NULL, (n), (s), \
 					  __FILE__, __LINE__)
extern	char *	estrdup_impl(const char *, const char *, int);
#define	estrdup(s) estrdup_impl((s), __FILE__, __LINE__)
#endif


extern	const char *	humanlogtime	(void);
extern	const char *	humantime	(time_t);
extern	char *	mfptoa		(uint32_t, uint32_t, short);
extern	char *	mfptoms		(uint32_t, uint32_t, short);
extern	const char * modetoa	(size_t);
extern	const char * eventstr	(int);
extern	const char * ceventstr	(int);
extern	const char * res_match_flags(u_short);
extern	const char * res_access_flags(u_short);
#ifdef HAVE_KERNEL_PLL
extern	const char * k_st_flags	(uint32_t);
#endif
extern	char *	statustoa	(int, int);
extern	sockaddr_u * netof	(sockaddr_u *);
extern	char *	numtoa		(uint32_t);
extern	char *	numtohost	(uint32_t);
extern	const char * socktoa	(const sockaddr_u *);
extern	const char * sockporttoa(const sockaddr_u *);
extern	u_short	sock_hash	(const sockaddr_u *);
extern	int	sockaddr_masktoprefixlen(const sockaddr_u *);
extern	const char * socktohost	(const sockaddr_u *);
extern	bool	octtoint	(const char *, u_long *);
extern	u_long	ranp2		(int);
extern	const char *refnumtoa	(sockaddr_u *);
extern	const char *refid_str	(uint32_t, int);

extern	bool	decodenetnum	(const char *, sockaddr_u *);

extern	void	signal_no_reset (int, void (*func)(int));
extern	void	set_ctrl_c_hook (ctrl_c_fn);

extern	void	getauthkeys 	(const char *);
extern	void	auth_agekeys	(void);
extern	void	rereadkeys	(void);

/*
 * Variable declarations for libntp.
 */

/* authkeys.c */
extern u_long	authkeynotfound;	/* keys not found */
extern u_long	authkeylookups;		/* calls to lookup keys */
extern u_long	authnumkeys;		/* number of active keys */
extern u_long	authkeyexpired;		/* key lifetime expirations */
extern u_long	authkeyuncached;	/* cache misses */
extern u_long	authencryptions;	/* calls to encrypt */
extern u_long	authdecryptions;	/* calls to decrypt */

extern int	authnumfreekeys;

/*
 * The key cache. We cache the last key we looked at here.
 */
extern keyid_t	cache_keyid;		/* key identifier */
extern int	cache_type;		/* key type */
extern uint8_t *	cache_secret;		/* secret */
extern u_short	cache_secretsize;	/* secret octets */
extern u_short	cache_flags;		/* KEY_ bit flags */

/* getopt.c */
extern char *	ntp_optarg;		/* global argument pointer */
extern int	ntp_optind;		/* global argv index */

/* lib_strbuf.c */
extern bool	ipv4_works;
extern bool	ipv6_works;

/* machines.c */
typedef void (*pset_tod_using)(const char *);
extern pset_tod_using	set_tod_using;

/* ssl_init.c */
#ifdef HAVE_OPENSSL
extern	void	ssl_init		(void);
extern	void	ssl_check_version	(void);
extern	bool	ssl_init_done;
#define	INIT_SSL()				\
	do {					\
		if (!ssl_init_done)		\
			ssl_init();		\
	} while (0)
#else	/* !HAVE_OPENSSL follows */
#define	INIT_SSL()		do {} while (0)
#endif
extern	int	keytype_from_text	(const char *,	size_t *);
extern	const char *keytype_name	(int);
extern	char *	getpass_keytype		(int);

/* strl-obsd.c */
#ifndef HAVE_STRLCPY		/* + */
/*
 * Copy src to string dst of size siz.  At most siz-1 characters
 * will be copied.  Always NUL terminates (unless siz == 0).
 * Returns strlen(src); if retval >= siz, truncation occurred.
 */
extern	size_t	strlcpy(char *dst, const char *src, size_t siz);
#endif
#ifndef HAVE_STRLCAT		/* + */
/*
 * Appends src to string dst of size siz (unlike strncat, siz is the
 * full size of dst, not space left).  At most siz-1 characters
 * will be copied.  Always NUL terminates (unless siz <= strlen(dst)).
 * Returns strlen(src) + MIN(siz, strlen(initial dst)).
 * If retval >= siz, truncation occurred.
 */
extern	size_t	strlcat(char *dst, const char *src, size_t siz);
#endif

/* systime.c */
extern double	sys_tick;		/* tick size or time to read */
extern double	measured_tick;		/* non-overridable sys_tick */
extern double	sys_fuzz;		/* min clock read latency */
extern bool	trunc_os_clock;		/* sys_tick > measured_tick */

/* use these as return values for sort-comparison functions */
#define COMPARE_GREATERTHAN	1
#define COMPARE_EQUAL		0
#define COMPARE_LESSTHAN	-1

#endif	/* GUARD_NTP_STDLIB_H */
