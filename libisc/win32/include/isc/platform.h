/*
 * Copyright (C) 2004, 2005, 2007-2009  Internet Systems Consortium, Inc. ("ISC")
 * Copyright (C) 2001  Internet Software Consortium.
 * SPDX-License-Identifier: ISC
 */

#ifndef GUARD_ISC_PLATFORM_H
#define GUARD_ISC_PLATFORM_H 1

/*****
 ***** Platform-dependent defines.
 *****/

#define ISC_PLATFORM_USETHREADS

/***
 *** Network.
 ***/

#if _MSC_VER > 1200
#define ISC_PLATFORM_HAVEIPV6
#define ISC_PLATFORM_HAVEIN6PKTINFO
#define ISC_PLATFORM_HAVESCOPEID
#endif
#define ISC_PLATFORM_NEEDPORTT
#undef MSG_TRUNC
#define ISC_PLATFORM_NEEDNTOP
#define ISC_PLATFORM_NEEDPTON

#ifndef ISC_PLATFORM_QUADFORMAT
#define ISC_PLATFORM_QUADFORMAT "I64"
#endif

#define ISC_PLATFORM_NEEDSTRSEP
#define ISC_PLATFORM_NEEDSTRLCPY
#define ISC_PLATFORM_NEEDSTRLCAT
#define ISC_PLATFORM_NEEDSTRLCPY

/*
 * Used to control how extern data is linked; needed for Win32 platforms.
 */
#define ISC_PLATFORM_USEDECLSPEC 1

/*
 * Define this here for now as winsock2.h defines h_errno
 * and we don't want to redeclare it.
 */
#define ISC_PLATFORM_NONSTDHERRNO

/*
 * Define if the platform has <sys/un.h>.
 */
#undef ISC_PLATFORM_HAVESYSUNH

/*
 * Defines for the noreturn attribute.
 */
#define ISC_PLATFORM_NORETURN_PRE __declspec(noreturn)
#define ISC_PLATFORM_NORETURN_POST

/*
 * Set up a macro for importing and exporting from the DLL
 *
 * To build static libraries on win32, #define ISC_STATIC_WIN
 */
#ifndef ISC_STATIC_WIN
#define ISC_DLLEXP	__declspec(dllexport)
#define ISC_DLLIMP	__declspec(dllimport)
#else
#define ISC_DLLEXP
#define ISC_DLLIMP
#endif

#ifdef LIBISC_EXPORTS
#define LIBISC_EXTERNAL_DATA		ISC_DLLEXP
#else
#define LIBISC_EXTERNAL_DATA		ISC_DLLIMP
#endif

#ifdef LIBISCCFG_EXPORTS
#define LIBISCCFG_EXTERNAL_DATA		ISC_DLLEXP
#else
#define LIBISCCFG_EXTERNAL_DATA		ISC_DLLIMP
#endif

#ifdef LIBISCCC_EXPORTS
#define LIBISCCC_EXTERNAL_DATA		ISC_DLLEXP
#else
#define LIBISCCC_EXTERNAL_DATA		ISC_DLLIMP
#endif

#ifdef LIBDNS_EXPORTS
#define LIBDNS_EXTERNAL_DATA		ISC_DLLEXP
#else
#define LIBDNS_EXTERNAL_DATA		ISC_DLLIMP
#endif

#ifdef LIBBIND9_EXPORTS
#define LIBBIND9_EXTERNAL_DATA		ISC_DLLEXP
#else
#define LIBBIND9_EXTERNAL_DATA		ISC_DLLIMP
#endif

#endif /* GUARD_ISC_PLATFORM_H */
