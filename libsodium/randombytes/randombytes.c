
#include <sys/types.h>

#include <assert.h>
#include <limits.h>
#include <stdint.h>

#include "randombytes.h"
#include "randombytes_sysrandom.h"

static const randombytes_implementation *implementation =
    &randombytes_sysrandom_implementation;

int
randombytes_set_implementation(randombytes_implementation *impl)
{
    implementation = impl;

    return 0;
}

const char *
randombytes_implementation_name(void)
{
    return implementation->implementation_name();
}

uint32_t
randombytes_random(void)
{
    return implementation->random();
}

void
randombytes_stir(void)
{
    if (implementation != NULL && implementation->stir != NULL) {
        implementation->stir();
    }
}

/*
 * randombytes_uniform() derives from OpenBSD's arc4random_uniform()
 * Copyright (c) 2008, Damien Miller <djm@openbsd.org>
 */
uint32_t
randombytes_uniform(const uint32_t upper_bound)
{
    uint32_t min;
    uint32_t r;

    if (implementation != NULL && implementation->uniform != NULL) {
        return implementation->uniform(upper_bound);
    }
    if (upper_bound < 2) {
        return 0;
    }
    min = (uint32_t) (-upper_bound % upper_bound);
    do {
        r = randombytes_random();
    } while (r < min);

    return r % upper_bound;
}

void
randombytes_buf(void * const buf, const size_t size)
{
    if (size > (size_t) 0U) {
        implementation->buf(buf, size);
    }
}

int
randombytes_close(void)
{
    if (implementation != NULL && implementation->close != NULL) {
        return implementation->close();
    }
    return 0;
}

void
randombytes(unsigned char * const buf, const unsigned long long buf_len)
{
    assert(buf_len <= SIZE_MAX);
    randombytes_buf(buf, (size_t) buf_len);
}
