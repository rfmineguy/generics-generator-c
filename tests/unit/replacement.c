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

static MunitResult test_replacement_add(const MunitParameter params[], void *userdata) {
	// 0. Setup a raw form of replacements
	//    [0] -> needle
	//    [1] -> with
	const char* replacements[4][2] = {
		{"needle", "with"},
		{"$T",     "int"},
		{"^T",     "int"},
		{"pod",    "something else"},
	};

	// 1. Create the replacement context (tested by 'test_replacement_create')
	replacement r = replacement_create();

	// 2. Setup initial replacement set using the 'replacement_add' (currently being tested)
	for (int i = 0; i < 4; i++)	
		replacement_add(&r, replacements[i][0], replacements[i][1]);

	// 3. Check to make sure that the content in the replacement context matches the data set
	//    it was created from
	munit_assert_int(r.replacements_count, ==, 4);
	for (int i = 0; i < 4; i++) {
		munit_assert_string_equal(r.replacements[i].needle, replacements[i][0]);
		munit_assert_string_equal(r.replacements[i].with,   replacements[i][1]);
	}

	replacement_free(&r);
	return MUNIT_OK;
}

static MunitResult test_replacement_forward(const MunitParameter params[], void *userdata) {
	replacement r = replacement_create();
	
	replacement r1 = replacement_create();
	replacement_add(&r1, "$T",    NULL);
	replacement_add(&r1, "^T",    NULL);
	replacement_add(&r1, "PRINT", "printf");

	replacement r2 = replacement_create();
	replacement_add(&r2, "$T", "string");
	replacement_add(&r2, "^T", "int");

	replacement r3 = replacement_forward(r1, r2);
	munit_assert_string_equal(r3.replacements[0].needle, "$T");
	munit_assert_string_equal(r3.replacements[0].with,   "string");
	munit_assert_string_equal(r3.replacements[1].needle, "^T");
	munit_assert_string_equal(r3.replacements[1].with,   "int");
	munit_assert_string_equal(r3.replacements[2].needle, "PRINT");
	munit_assert_string_equal(r3.replacements[2].with,   "printf");
	
	replacement_free(&r);
	return MUNIT_OK;
}
#endif
