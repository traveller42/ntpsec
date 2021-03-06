/*
 * refclock_conf.c - reference clock configuration
 */
#include <config.h>

#include <stdio.h>
#include <sys/types.h>

#include "ntpd.h"
#include "ntp_refclock.h"
#include "ntp_stdlib.h"

static struct refclock refclock_none = {
	noentry, noentry, noentry, noentry, noentry, noentry, noentry
};

#ifdef CLOCK_LOCAL
extern	struct refclock	refclock_local;
#else
#define	refclock_local	refclock_none
#endif

#ifdef CLOCK_CHU
extern	struct refclock	refclock_chu;
#else
#define	refclock_chu	refclock_none
#endif

#ifdef CLOCK_SPECTRACOM
extern	struct refclock	refclock_spectracom;
#else
#define	refclock_spectracom	refclock_none
#endif

#ifdef CLOCK_PARSE
extern	struct refclock	refclock_parse;
#else
#define	refclock_parse	refclock_none
#endif

#if defined(CLOCK_MX4200) && defined(HAVE_PPSAPI)
extern	struct refclock	refclock_mx4200;
#else
#define	refclock_mx4200	refclock_none
#endif

#ifdef CLOCK_AS2201
extern	struct refclock	refclock_as2201;
#else
#define	refclock_as2201	refclock_none
#endif

#ifdef CLOCK_ARBITER
extern  struct refclock refclock_arbiter;
#else
#define refclock_arbiter refclock_none
#endif

#ifdef CLOCK_IRIG
extern	struct refclock	refclock_irig;
#else
#define refclock_irig	refclock_none
#endif

#ifdef CLOCK_TRUETIME
extern	struct refclock	refclock_true;
#else
#define	refclock_true	refclock_none
#endif

#ifdef CLOCK_ACTS
extern	struct refclock	refclock_acts;
#else
#define refclock_acts	refclock_none
#endif

#ifdef CLOCK_NMEA
extern	struct refclock refclock_nmea;
#else
#define	refclock_nmea	refclock_none
#endif

#if defined (CLOCK_ATOM) && defined(HAVE_PPSAPI)
extern	struct refclock	refclock_atom;
#else
#define refclock_atom	refclock_none
#endif

#ifdef CLOCK_HPGPS
extern	struct refclock	refclock_hpgps;
#else
#define	refclock_hpgps	refclock_none
#endif

#ifdef CLOCK_ARCRON_MSF
extern	struct refclock refclock_arc;
#else
#define refclock_arc refclock_none
#endif

#ifdef CLOCK_SHM
extern	struct refclock refclock_shm;
#else
#define refclock_shm refclock_none
#endif

#ifdef CLOCK_PALISADE 
extern	struct refclock refclock_palisade;
#else
#define refclock_palisade refclock_none
#endif

#if defined(CLOCK_ONCORE)
extern	struct refclock refclock_oncore;
#else
#define refclock_oncore refclock_none
#endif

#if defined(CLOCK_JUPITER) && defined(HAVE_PPSAPI)
extern	struct refclock refclock_jupiter;
#else
#define refclock_jupiter refclock_none
#endif

#if defined(CLOCK_DUMBCLOCK)
extern struct refclock refclock_dumbclock;
#else
#define refclock_dumbclock refclock_none
#endif

#ifdef CLOCK_PCF
extern	struct refclock	refclock_pcf;
#else
#define	refclock_pcf	refclock_none
#endif

#ifdef CLOCK_HOPF_SERIAL
extern	struct refclock	refclock_hopfser;
#else
#define	refclock_hopfser refclock_none
#endif

#ifdef CLOCK_HOPF_PCI
extern	struct refclock	refclock_hopfpci;
#else
#define	refclock_hopfpci refclock_none
#endif

#ifdef CLOCK_JJY
extern	struct refclock	refclock_jjy;
#else
#define	refclock_jjy refclock_none
#endif

#ifdef CLOCK_ZYFER
extern	struct refclock	refclock_zyfer;
#else
#define	refclock_zyfer refclock_none
#endif

#ifdef CLOCK_NEOCLOCK4X
extern	struct refclock	refclock_neoclock4x;
#else
#define	refclock_neoclock4x	refclock_none
#endif

#ifdef CLOCK_TSYNCPCI
extern struct refclock refclock_tsyncpci;
#else
#define refclock_tsyncpci refclock_none
#endif

#if defined(CLOCK_GPSDJSON) && !defined(SYS_WINNT) 
extern struct refclock refclock_gpsdjson;
#else
#define refclock_gpsdjson refclock_none
#endif
/*
 * Order is clock_start(), clock_shutdown(), clock_poll(),
 * clock_control(), clock_init(), clock_buginfo, clock_flags;
 *
 * Types are defined in ntp.h.  The index must match this.
 */
struct refclock * const refclock_conf[] = {
	&refclock_none,		/* 0 REFCLK_NONE */
	&refclock_local,	/* 1 REFCLK_LOCAL */
	&refclock_none,		/* 2 was: REFCLK_GPS_TRAK */
	&refclock_none,		/* 3 was: REFCLK_WWV_PST */
	&refclock_spectracom, 	/* 4 REFCLK_SPECTRACOM */
	&refclock_true,		/* 5 REFCLK_TRUETIME */
	&refclock_irig,		/* 6 REFCLK_IRIG_AUDIO */
	&refclock_chu,		/* 7 REFCLK_CHU_AUDIO */
	&refclock_parse,	/* 8 REFCLK_PARSE */
	&refclock_mx4200,	/* 9 REFCLK_GPS_MX4200 */
	&refclock_as2201,	/* 10 REFCLK_GPS_AS2201 */
	&refclock_arbiter,	/* 11 REFCLK_GPS_ARBITER */
	&refclock_none,		/* 12 was: REFCLK_IRIG_TPRO */
	&refclock_none,		/* 13 was: REFCLK_ATOM_LEITCH */
	&refclock_none,		/* 14 was: REFCLOCK_MSF_EES */
	&refclock_none,		/* 15 was: OLD TrueTime GPS/TM-TMD Receiver */
	&refclock_none,		/* 16 was: REFCLK_IRIG_BANCOMM */
	&refclock_none,		/* 17 was: REFCLK_GPS_DATUM */
	&refclock_acts,		/* 18 REFCLK_ACTS */
	&refclock_none,		/* 19 was: REFCLK_WWV_HEATH */
	&refclock_nmea,		/* 20 REFCLK_GPS_NMEA */
	&refclock_none,		/* 21 was: REFCLK_GPS_VME */
	&refclock_atom,		/* 22 REFCLK_ATOM_PPS */
	&refclock_none,		/* 23 not used */
	&refclock_none,		/* 24 not used */
	&refclock_none,		/* 25 not used */
	&refclock_hpgps,	/* 26 REFCLK_GPS_HP */
	&refclock_arc, 		/* 27 REFCLK_ARCRON_MSF */
	&refclock_shm,		/* 28 REFCLK_SHM */
	&refclock_palisade,	/* 29 REFCLK_PALISADE */
	&refclock_oncore,	/* 30 REFCLK_ONCORE */
	&refclock_jupiter,	/* 31 REFCLK_GPS_JUPITER */
	&refclock_none,		/* 32 was: REFCLK_CHRONOLOG */
	&refclock_dumbclock,	/* 33 REFCLK_DUMBCLOCK */
	&refclock_none,		/* 34 was: REFCLOCK_ULINK */
	&refclock_pcf,		/* 35 REFCLOCK_PCF */
	&refclock_none,		/* 36 was: REFCLOCK_WWV_AUDIO */
	&refclock_none,		/* 37 was: REFCLOCK_FG */
	&refclock_hopfser,	/* 38 REFCLK_HOPF_SERIAL */
	&refclock_hopfpci,	/* 39 REFCLK_HOPF_PCI */
	&refclock_jjy,		/* 40 REFCLK_JJY */
	&refclock_none,		/* 41 was: REFCLK_TT560 */
	&refclock_zyfer,	/* 42 REFCLK_ZYFER */
	&refclock_none,		/* 43 was: REFCLK_RIPENCC */
	&refclock_neoclock4x,	/* 44 REFCLK_NEOCLOCK4X */
	&refclock_tsyncpci,	/* 45 REFCLK_TSYNCPCI */
	&refclock_gpsdjson	/* 46 REFCLK_GPSDJSON */
};

uint8_t num_refclock_conf = sizeof(refclock_conf)/sizeof(struct refclock *);
