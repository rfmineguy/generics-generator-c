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

#endif
