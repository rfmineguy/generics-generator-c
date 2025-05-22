#ifndef GENERATOR_TEST_C
#define GENERATOR_TEST_C
#include "alltests.h"
#include "../../gengen.h"
#include "../lib/util.h"
#include <unistd.h>

MunitResult test_generator_linkedlist_string(const MunitParameter params[], void *userdata) {
	ctemplate linkedlist = template_create();
	template_addfile       (&linkedlist, "linked_list.htpl", "linked_list_$T.h");
	template_addfile       (&linkedlist, "linked_list.ctpl", "linked_list_$T.c");
	template_addreplacement(&linkedlist, "$T",     NULL);
	template_addreplacement(&linkedlist, "^T",     NULL);
	template_addreplacement(&linkedlist, "PRINTF", "printf");
	template_addreplacement(&linkedlist, "FREE",   "free");
	template_addreplacement(&linkedlist, "CALLOC", "calloc");
	template_addreplacement(&linkedlist, "HEADER", "stdint.h");

	replacement ll_int = replacement_create();
	replacement_add(&ll_int, "$T", "string");
	replacement_add(&ll_int, "^T", "const char*");

	generator_run(settings_custom(.search_paths=paths("templates"), .outdir="output"), linkedlist, ll_int);

	munit_assert_file_exists("output/linked_list_string.h");
	munit_assert_file_exists("output/linked_list_string.c");

	munit_assert_int(util_count_symbols("templates/linked_list.htpl", "$T"), ==, util_count_symbols("output/linked_list_string.h", "string"));
	munit_assert_int(util_count_symbols("templates/linked_list.htpl", "^T"), ==, util_count_symbols("output/linked_list_string.h", "const char*"));

	template_free(&linkedlist);
	replacement_free(&ll_int);

	return MUNIT_OK;
}

	return MUNIT_OK;
}

MunitTest generator_tests[] = {
	{ "/linkedlist_string", test_generator_linkedlist_string, NULL, NULL, MUNIT_TEST_OPTION_NONE, NULL},
  { NULL, NULL, NULL, NULL, MUNIT_TEST_OPTION_NONE, NULL }
};

#endif
