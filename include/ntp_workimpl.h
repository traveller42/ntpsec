/*
 * ntp_workimpl.h - selects worker child implementation
 */
#ifndef GUARD_NTP_WORKIMPL_H
#define GUARD_NTP_WORKIMPL_H

/*
 * Some systems do not support fork() and don't have an alternate
 * threads implementation of ntp_intres.  Such systems are limited
 * to using numeric IP addresses.
 */
#if defined(SYS_WINNT)
# define USE_WORK_THREAD
#else
# define USE_WORK_THREAD
# define USE_WORK_PIPE
#endif

#if defined(USE_WORK_THREAD)
/* Guards are not strictly necessary, bit keep them as documentation */
# define USE_WORKER
#endif

#endif	/* GUARD_!NTP_WORKIMPL_H */
