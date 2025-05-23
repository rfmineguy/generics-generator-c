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

static MunitResult test_replacement_forward_1(const MunitParameter params[], void *userdata) {
	replacement r1 = replacement_create();
	replacement_add(&r1, "$T",    NULL);
	replacement_add(&r1, "^T",    NULL);
	replacement_add(&r1, "LORN",  NULL);
	replacement_add(&r1, "PRINT", "printf");

	replacement r2 = replacement_create();
	replacement_add(&r2, "$T", "string");
	replacement_add(&r2, "^T", "int");
	replacement_add(&r2, "X",  "poop");

	forward_table fwd = forward_table_create();
	forward_table_forward(&fwd, fwd(.symbol="^T", .as="$T"));
	forward_table_forward(&fwd, fwd(.symbol="$T", .as="^T"));
	forward_table_forward(&fwd, fwd(.symbol="X",  .as="LORN"));

	replacement r3 = replacement_forward(r1, r2, fwd);
	munit_assert_replacement_contains(r3, "$T", "int");
	munit_assert_replacement_contains(r3, "^T", "string");
	munit_assert_replacement_contains(r3, "LORN", "poop");
	munit_assert_replacement_contains(r3, "PRINT", "printf");
	
	replacement_free(&r1);
	replacement_free(&r2);
	replacement_free(&r3);
	return MUNIT_OK;
}

static MunitResult test_replacement_forward_2(const MunitParameter params[], void *userdata) {
	replacement r1 = replacement_create();
	replacement_add(&r1, "$X",      NULL);
	replacement_add(&r1, "^X",      NULL);
	replacement_add(&r1, "$T",      NULL);
	replacement_add(&r1, "^T",      NULL);
	replacement_add(&r1, "CALLOC",  "calloc");
	replacement_add(&r1, "PRINT",   "printf");

	replacement r2 = replacement_create();
	replacement_add(&r2, "$T", "string");
	replacement_add(&r2, "^T", "int");
	replacement_add(&r2, "X",  "poop");

	forward_table fwd = forward_table_create();
	forward_table_forward(&fwd, fwd(.symbol="^T", .as="$T"));
	forward_table_forward(&fwd, fwd(.symbol="$T", .as="^T"));
	forward_table_forward(&fwd, fwd(.symbol="X",  .as="$X"));
	forward_table_forward(&fwd, fwd(.symbol="X",  .as="^X"));
	forward_table_forward(&fwd, fwd(.symbol="X",  .as="CALLOC"));

	replacement_item r_;
	replacement r3 = replacement_forward(r1, r2, fwd);
	munit_assert_replacement_contains(r3, "$X", "poop");
	munit_assert_replacement_contains(r3, "^X", "poop");
	munit_assert_replacement_contains(r3, "$T", "int");
	munit_assert_replacement_contains(r3, "^T", "string");
	munit_assert_replacement_contains(r3, "CALLOC", "calloc");
	munit_assert_replacement_contains(r3, "PRINT", "printf");
	
	replacement_free(&r1);
	replacement_free(&r2);
	replacement_free(&r3);
	return MUNIT_OK;
}
	return MUNIT_OK;
}

static MunitResult test_replacement_get(const MunitParameter params[], void *userdata) {
	const char* mock_cursor = "$T()...";

	replacement r = replacement_create();
	replacement_add(&r, "$T", "string");
	munit_assert_not_null(replacement_get(&r, mock_cursor));

	mock_cursor = "hello";
	munit_assert_ptr_null(replacement_get(&r, mock_cursor));

	replacement_free(&r);
	return MUNIT_OK;
}

MunitTest replacement_tests[] = {
	{ "/create", 			   test_replacement_create,   NULL, NULL, MUNIT_TEST_OPTION_NONE, NULL},
	{ "/forward", 			 test_replacement_forward , NULL, NULL, MUNIT_TEST_OPTION_NONE, NULL},
	{ "/add", 					 test_replacement_add ,     NULL, NULL, MUNIT_TEST_OPTION_NONE, NULL},
	{ "/get",     			 test_replacement_get     , NULL, NULL, MUNIT_TEST_OPTION_NONE, NULL},

  { NULL, NULL, NULL, NULL, MUNIT_TEST_OPTION_NONE, NULL }
};

#endif
