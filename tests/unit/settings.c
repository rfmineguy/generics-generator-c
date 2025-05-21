#ifndef SETTINGS_TEST_C
#define SETTINGS_TEST_C
#include "alltests.h"

static MunitResult test_settings_custom_empty_paths(const MunitParameter params[], void *userdata) {
	generator_settings settings = settings_custom(.search_paths=paths(), .outdir=".");
	munit_assert_int(settings.path_count, ==, 0);
	munit_assert_string_equal(".", settings.outdir);
	return MUNIT_OK;
}

static MunitResult test_settings_custom_with_single_path(const MunitParameter params[], void *userdata) {
	generator_settings settings = settings_custom(.search_paths=paths("demo_templates"), .outdir=".");
	munit_assert_int(settings.path_count, ==, 1);
	munit_assert_string_equal("demo_templates", settings.search_paths[0]);
	munit_assert_string_equal(".", settings.outdir);
	return MUNIT_OK;
}

static MunitResult test_settings_custom_with_multiple_paths(const MunitParameter params[], void *userdata) {
	generator_settings settings = settings_custom(.search_paths=paths("demo_templates", "."), .outdir=".");
	munit_assert_int(settings.path_count, ==, 2);
	munit_assert_string_equal("demo_templates", settings.search_paths[0]);
	munit_assert_string_equal(".", settings.search_paths[1]);
	munit_assert_string_equal(".", settings.outdir);
	return MUNIT_OK;
}

MunitTest settings_tests[] = {
	{ "/custom_empty_paths", 			   test_settings_custom_empty_paths, NULL, NULL, MUNIT_TEST_OPTION_NONE, NULL},
	{ "/custom_with_single_path",    test_settings_custom_with_single_path, NULL, NULL, MUNIT_TEST_OPTION_NONE, NULL},
	{ "/custom_with_multiple_paths", test_settings_custom_with_multiple_paths, NULL, NULL, MUNIT_TEST_OPTION_NONE, NULL},
  { NULL, NULL, NULL, NULL, MUNIT_TEST_OPTION_NONE, NULL }
};

#endif
