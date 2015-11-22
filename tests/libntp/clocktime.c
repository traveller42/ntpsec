extern "C" {
#include "unity.h"
#include "unity_fixture.h"
}

TEST_GROUP(clocktime);

TEST_SETUP(clocktime) {}

TEST_TEAR_DOWN(clocktime) {}

#include "libntptest.h"

// ---------------------------------------------------------------------
// test fixture
//
// The clocktimeTest uses the NTP calendar feature to use a mockup
// function for getting the current system time, so the tests are not
// dependent on the actual system time.

class clocktimeTest : public libntptest {
	virtual void SetUp();
	virtual void TearDown();
};

void clocktimeTest::SetUp()
{
    ntpcal_set_timefunc(timefunc);
    settime(2000, 1, 1, 0, 0, 0);
}

void clocktimeTest::TearDown()
{
    ntpcal_set_timefunc(NULL);
}

// ---------------------------------------------------------------------
// test cases

TEST(clocktime, CurrentYear) {
	// Timestamp: 2010-06-24 12:50:00Z
	const u_int32 timestamp = 3486372600UL;
	const u_int32 expected	= timestamp; // exactly the same.

	const int yday=175, hour=12, minute=50, second=0, tzoff=0;

	u_long yearstart=0;
	u_int32 actual;

	TEST_ASSERT_TRUE(clocktime(yday, hour, minute, second, tzoff, timestamp,
						  &yearstart, &actual));
	TEST_ASSERT_EQUAL(expected, actual);
}

TEST(clocktime, CurrentYearFuzz) {
	/* 
	 * Timestamp (rec_ui) is: 2010-06-24 12:50:00
	 * Time sent into function is 12:00:00.
	 *
	 * Since the fuzz is rather small, we should get a NTP
	 * timestamp for the 12:00:00 time.
	 */

	const u_int32 timestamp = 3486372600UL; // 2010-06-24 12:50:00Z
	const u_int32 expected	= 3486369600UL; // 2010-06-24 12:00:00Z

	const int yday=175, hour=12, minute=0, second=0, tzoff=0;

	u_long yearstart=0;
	u_int32 actual;

	TEST_ASSERT_TRUE(clocktime(yday, hour, minute, second, tzoff, timestamp,
						  &yearstart, &actual));
	TEST_ASSERT_EQUAL(expected, actual);
}

TEST(clocktime, TimeZoneOffset) {
	/*
	 * Timestamp (rec_ui) is: 2010-06-24 12:00:00 +0800
	 * (which is 2010-06-24 04:00:00Z)
	 *
	 * Time sent into function is 04:00:00 +0800
	 */
	const u_int32 timestamp = 3486369600UL;
	const u_int32 expected	= timestamp;

	const int yday=175, hour=4, minute=0, second=0, tzoff=8;

	u_long yearstart=0;
	u_int32 actual;

	TEST_ASSERT_TRUE(clocktime(yday, hour, minute, second, tzoff, timestamp,
						  &yearstart, &actual));
	TEST_ASSERT_EQUAL(expected, actual);
}

TEST(clocktime, WrongYearStart) {
	/* 
	 * Timestamp (rec_ui) is: 2010-01-02 11:00:00Z
	 * Time sent into function is 11:00:00.
	 * Yearstart sent into function is the yearstart of 2009!
	 */
	const u_int32 timestamp = 3471418800UL;
	const u_int32 expected	= timestamp;

	const int yday=2, hour=11, minute=0, second=0, tzoff=0;

	u_long yearstart = 302024100UL; // Yearstart of 2009.
	u_int32 actual;

	TEST_ASSERT_TRUE(clocktime(yday, hour, minute, second, tzoff, timestamp,
						  &yearstart, &actual));
	TEST_ASSERT_EQUAL(expected, actual);
}

TEST(clocktime, PreviousYear) {
	/*
	 * Timestamp is: 2010-01-01 01:00:00Z
	 * Time sent into function is 23:00:00
	 * (which is meant to be 2009-12-31 23:00:00Z)
	 */
	const u_int32 timestamp = 3471296400UL;
	const u_int32 expected	= 3471289200UL;

	const int yday=365, hour=23, minute=0, second=0, tzoff=0;

	u_long yearstart = 0;
	u_int32 actual;

	TEST_ASSERT_TRUE(clocktime(yday, hour, minute, second, tzoff, timestamp,
						  &yearstart, &actual));
	TEST_ASSERT_EQUAL(expected, actual);
}

TEST(clocktime, NextYear) {
	/*
	 * Timestamp is: 2009-12-31 23:00:00Z
	 * Time sent into function is 01:00:00
	 * (which is meant to be 2010-01-01 01:00:00Z)
	 */
	const u_int32 timestamp = 3471289200UL;
	const u_int32 expected	= 3471296400UL;

	const int yday=1, hour=1, minute=0, second=0, tzoff=0;
	u_long yearstart = 0;
	u_int32 actual;

	TEST_ASSERT_TRUE(clocktime(yday, hour, minute, second, tzoff, timestamp,
						  &yearstart, &actual));
	TEST_ASSERT_EQUAL(expected, actual);
}

TEST(clocktime, NoReasonableConversion) {
	/* Timestamp is: 2010-01-02 11:00:00Z */
	const u_int32 timestamp = 3471418800UL;

	const int yday=100, hour=12, minute=0, second=0, tzoff=0;
	u_long yearstart = 0;
	u_int32 actual;

	TEST_ASSERT_FALSE(clocktime(yday, hour, minute, second, tzoff, timestamp,
						   &yearstart, &actual));
}

TEST(clocktime, AlwaysInLimit) {
	/* Timestamp is: 2010-01-02 11:00:00Z */
	const u_int32 timestamp = 3471418800UL;
	const u_short prime_incs[] = { 127, 151, 163, 179 };
	int	cyc;
	int	yday;
	u_char	whichprime;
	u_short	ydayinc;
	int	hour;
	int	minute;
	int	second;
	u_long	yearstart;
	u_int32	actual;
	u_int32	diff;

	yearstart = 0;
	for (cyc = 0; cyc < 5; cyc++) {
		settime(1900 + cyc * 65, 1, 1, 0, 0, 0);
		for (yday = -26000; yday < 26000; yday += ydayinc) {
			whichprime = abs(yday) % COUNTOF(prime_incs);
			ydayinc = prime_incs[whichprime];
			for (hour = -204; hour < 204; hour += 2) {
				for (minute = -60; minute < 60; minute++) {
					clocktime(yday, hour, minute, 30, 0,
						  timestamp, &yearstart, &actual);
					diff = actual - timestamp;
					if (diff >= 0x80000000UL)
						diff = ~diff + 1;
					TEST_ASSERT_LESS_THAN_OR_EQUAL(diff, (183u * SECSPERDAY));
				}
			}
		}
	}
}

TEST_GROUP_RUNNER(clocktime) {
	RUN_TEST_CASE(clocktime, CurrentYear);
	RUN_TEST_CASE(clocktime, CurrentYearFuzz);
	RUN_TEST_CASE(clocktime, TimeZoneOffset);
	RUN_TEST_CASE(clocktime, WrongYearStart);
	RUN_TEST_CASE(clocktime, PreviousYear);
	RUN_TEST_CASE(clocktime, NextYear);
	RUN_TEST_CASE(clocktime, NoReasonableConversion);
	RUN_TEST_CASE(clocktime, AlwaysInLimit);
}