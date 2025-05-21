#ifndef REPLACEMENT_TEST_C
#define REPLACEMENT_TEST_C
#include "alltests.h"

static MunitResult test_replacement_create(const MunitParameter params[], void *userdata) {
	replacement r = replacement_create();
	munit_assert_ptr_null(r.replacements);
	munit_assert_int(r.replacements_count, ==, 0);
	munit_assert_int(r.replacements_capacity, ==, 10);
	replacement_free(&r);
	return MUNIT_OK;
}
#endif
