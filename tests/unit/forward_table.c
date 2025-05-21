#ifndef FORWARD_TABLE_TEST_C
#define FORWARD_TABLE_TEST_C
#include "alltests.h"

static MunitResult test_forward_table_create(const MunitParameter params[], void *userdata) {
	forward_table fwd_table = forward_table_create();
	munit_assert_null(fwd_table.fwd_items);
	munit_assert_int(fwd_table.fwd_items_count, ==, 0);
	munit_assert_int(fwd_table.fwd_items_capacity, ==, 10);
	forward_table_free(&fwd_table);
	return MUNIT_OK;
}

static MunitResult test_forward_table_forward(const MunitParameter params[], void *userdata) {
	forward_table fwd_table = forward_table_create();
	
	forward_table_forward(&fwd_table, fwd(.symbol="$T", .as="$T"));
	forward_table_forward(&fwd_table, fwd(.symbol="^T", .as="^T"));

	munit_assert_int(fwd_table.fwd_items_count, ==, 2);
	munit_assert_string_equal(fwd_table.fwd_items[0].symbol, "$T");
	munit_assert_string_equal(fwd_table.fwd_items[0].as,     "$T");
	munit_assert_string_equal(fwd_table.fwd_items[1].symbol, "^T");
	munit_assert_string_equal(fwd_table.fwd_items[1].as,     "^T");

	forward_table_free(&fwd_table);
	return MUNIT_OK;
}

MunitTest forward_table_tests[] = {
	{ "/create",     test_forward_table_create, NULL, NULL, MUNIT_TEST_OPTION_NONE, NULL},
	{ "/forward",    test_forward_table_forward, NULL, NULL, MUNIT_TEST_OPTION_NONE, NULL},
  { NULL, NULL, NULL, NULL, MUNIT_TEST_OPTION_NONE, NULL }
};

#endif
