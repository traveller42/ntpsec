/*
 * Define malloc and friends.
 */
#ifndef GUARD_NTP_MALLOC_H
#define GUARD_NTP_MALLOC_H

#include <stdlib.h>

/*
 * Deal with platform differences declaring alloca()
 * This comes nearly verbatim from:
 *
 * http://www.gnu.org/software/autoconf/manual/autoconf.html#Particular-Functions
 *
 * The only modifications were to remove C++ support and guard against
 * redefining alloca.
 */
#ifdef HAVE_ALLOCA_H
# include <alloca.h>
#elif defined __GNUC__
# ifndef alloca
#  define alloca __builtin_alloca
# endif
#elif defined _AIX
# ifndef alloca
#  define alloca __alloca
# endif
#elif defined _MSC_VER
# include <malloc.h>
# ifndef alloca
#  define alloca _alloca
# endif
#else
# include <stddef.h>
void * alloca(size_t);
#endif

#ifdef EREALLOC_IMPL
# define EREALLOC_CALLSITE	/* preserve __FILE__ and __LINE__ */
#else
# define EREALLOC_IMPL(ptr, newsz, filenm, loc) \
	 realloc(ptr, (newsz))
#endif

#include <strings.h>

#define zero_mem(p, s)		memset(p, 0, s)

#define ZERO(var)		zero_mem(&(var), sizeof(var))

#endif	/* GUARD_NTP_MALLOC_H */
