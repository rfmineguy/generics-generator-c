#ifndef SETTINGS_TEST_C
#define SETTINGS_TEST_C
#include "alltests.h"

static MunitResult test_settings_custom_empty_paths(const MunitParameter params[], void *userdata) {
	generator_settings settings = settings_custom(.search_paths=paths(), .outdir=".");
	munit_assert_int(settings.path_count, ==, 0);
	munit_assert_string_equal(".", settings.outdir);
	return MUNIT_OK;
}

#endif
