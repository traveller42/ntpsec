/*
 * vint64ops.c - operations on 'vint64' values
 *
 * Written by Juergen Perlinger (perlinger@ntp.org) for the NTP project.
 * Copyright 2015 by the NTPsec project contributors
 * SPDX-License-Identifier: NTP
 * ----------------------------------------------------------------------
 * This is an attempt to get the vint64 calculations stuff centralised.
 */

#include <config.h>
#include <stdlib.h>
#include <ctype.h>
#include <string.h>
#include <errno.h>

#include "ntp_types.h"
#include "ntp_fp.h"
#include "vint64ops.h"

/* ---------------------------------------------------------------------
 * GCC is rather sticky with its 'const' attribute. We have to do it more
 * explicit than with a cast if we want to get rid of a CONST qualifier.
 * Greetings from the PASCAL world, where casting was only possible via
 * untagged unions...
 */
static inline void*
noconst(
	const void* ptr
	)
{
	union {
		const void * cp;
		void *       vp;
	} tmp;
	tmp.cp = ptr;
	return tmp.vp;
}

/* -------------------------------------------------------------------------*/

vint64
strtouv64(
	const char * begp,
	char **      endp,
	int          base
	)
{
	vint64  res;
	uint8_t  digit;
	int     sig, num;
	const uint8_t *src;
	
	num = sig = 0;
	src = (const uint8_t*)begp;
	while (isspace(*src))
		src++;

	if (*src == '-') {
		src++;
		sig = 1;
	} else  if (*src == '+') {
		src++;
	}

	if (base == 0) {
		base = 10;
		if (*src == '0') {
			base = 8;
			if (toupper(*++src) == 'X') {
				src++;
				base = 16;
			}
		}
	} else if (base == 16) { /* remove optional leading '0x' or '0X' */
		if (src[0] == '0' && toupper(src[1]) == 'X')
			src += 2;
	} else if (base <= 2 || base > 36) {
		memset(&res, 0xFF, sizeof(res));
		errno = ERANGE;
		return res;
	}
	
	memset(&res, 0, sizeof(res));
	while (*src) {
		if (isdigit(*src))
			digit = *src - '0';
		else if (isupper(*src))
			digit = *src - 'A' + 10;
		else if (islower(*src))
			digit = *src - 'a' + 10;
		else
			break;
		if (digit >= base)
			break;
		num = 1;
		res.Q_s = res.Q_s * base + digit;
		src++;
	}
	if (!num)
		errno = EINVAL;
	if (endp)
		*endp = (char*)noconst(src);
	if (sig)
		M_NEG(res.D_s.hi, res.D_s.lo);
	return res;
}

/* -------------------------------------------------------------------------*/

int
icmpv64(
	const vint64 * lhs,
	const vint64 * rhs
	)
{
	int res;

	res = (lhs->q_s > rhs->q_s)
	    - (lhs->q_s < rhs->q_s);

	return res;
}

/* -------------------------------------------------------------------------*/

int
ucmpv64(
	const vint64 * lhs,
	const vint64 * rhs
	)
{
	int res;
	
	res = (lhs->Q_s > rhs->Q_s)
	    - (lhs->Q_s < rhs->Q_s);

	return res;
}

/* -------------------------------------------------------------------------*/

vint64
addv64(
	const vint64 *lhs,
	const vint64 *rhs
	)
{
	vint64 res;

	res.Q_s = lhs->Q_s + rhs->Q_s;

	return res;
}

/* -------------------------------------------------------------------------*/

vint64
subv64(
	const vint64 *lhs,
	const vint64 *rhs
	)
{
	vint64 res;

	res.Q_s = lhs->Q_s - rhs->Q_s;

	return res;
}

/* -------------------------------------------------------------------------*/

vint64
addv64i32(
	const vint64 * lhs,
	int32_t        rhs
	)
{
	vint64 res;

	res = *lhs;
	res.q_s += rhs;

	return res;
}

/* -------------------------------------------------------------------------*/

vint64
subv64i32(
	const vint64 * lhs,
	int32_t        rhs
	)
{
	vint64 res;

	res = *lhs;
	res.q_s -= rhs;

	return res;
}

/* -------------------------------------------------------------------------*/

vint64
addv64u32(
	const vint64 * lhs,
	uint32_t       rhs
	)
{
	vint64 res;

	res = *lhs;
	res.Q_s += rhs;

	return res;
}

/* -------------------------------------------------------------------------*/

vint64
subv64u32(
	const vint64 * lhs,
	uint32_t       rhs
	)
{
	vint64 res;

	res = *lhs;
	res.Q_s -= rhs;

	return res;
}
