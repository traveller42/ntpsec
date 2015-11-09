/*
 * Copyright (C) 2004, 2007  Internet Systems Consortium, Inc. ("ISC")
 * Copyright (C) 1999-2001  Internet Software Consortium.
 * Copyright 2015 by the NTPsec project contributors
 * SPDX-License-Identifier: ISC
 */

#ifndef GUARD_ISC_INT_H
#define GUARD_ISC_INT_H 1

#define _INTEGRAL_MAX_BITS 64
#include <limits.h>

typedef __int8				isc_int8_t;
typedef unsigned __int8			isc_uint8_t;
typedef __int16				isc_int16_t;
typedef unsigned __int16		isc_uint16_t;
typedef __int32				isc_int32_t;
typedef unsigned __int32		isc_uint32_t;
typedef __int64				isc_int64_t;
typedef unsigned __int64		isc_uint64_t;

#define ISC_INT8_MIN	-128
#define ISC_INT8_MAX	127
#define ISC_UINT8_MAX	255

#define ISC_INT16_MIN	-32768
#define ISC_INT16_MAX	32767
#define ISC_UINT16_MAX	65535

/*
 * Note that "int" is 32 bits on all currently supported Unix-like operating
 * systems, but "long" can be either 32 bits or 64 bits, thus the 32 bit
 * constants are not qualified with "L".
 */
#define ISC_INT32_MIN	_I32_MIN
#define ISC_INT32_MAX	_I32_MAX
#define ISC_UINT32_MAX	_UI32_MAX

#define ISC_INT64_MIN	_I64_MIN
#define ISC_INT64_MAX	_I64_MAX
#define ISC_UINT64_MAX	_UI64_MAX

#endif /* GUARD_ISC_INT_H */