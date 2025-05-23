#ifndef GENERATOR_TEST_C
#define GENERATOR_TEST_C
#include "alltests.h"
#include "../../gengen.h"
#include "../lib/util.h"
#include <unistd.h>

#define LINKEDLIST "linkedlist"
#define QUEUE "queue"

MunitResult test_generator_linkedlist_string(const MunitParameter params[], void *userdata) {
	ctemplate linkedlist = template_create(LINKEDLIST);
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

MunitResult test_generator_linkedlist_long(const MunitParameter params[], void *userdata) {
	ctemplate linkedlist = template_create(LINKEDLIST);
	template_addfile       (&linkedlist, "linked_list.htpl", "linked_list_$T.h");
	template_addfile       (&linkedlist, "linked_list.ctpl", "linked_list_$T.c");
	template_addreplacement(&linkedlist, "$T",     NULL);
	template_addreplacement(&linkedlist, "^T",     NULL);
	template_addreplacement(&linkedlist, "PRINTF", "printf");
	template_addreplacement(&linkedlist, "FREE",   "free");
	template_addreplacement(&linkedlist, "CALLOC", "calloc");
	template_addreplacement(&linkedlist, "HEADER", "stdint.h");

	replacement ll_age = replacement_create();
	replacement_add(&ll_age, "$T", "age");
	replacement_add(&ll_age, "^T", "long");

	generator_run(settings_custom(.search_paths=paths("templates"), .outdir="output"), linkedlist, ll_age);

	munit_assert_file_exists("output/linked_list_age.h");
	munit_assert_file_exists("output/linked_list_age.c");

	munit_assert_int(util_count_symbols("templates/linked_list.htpl", "$T"), ==, util_count_symbols("output/linked_list_age.h", "age"));
	munit_assert_int(util_count_symbols("templates/linked_list.htpl", "^T"), ==, util_count_symbols("output/linked_list_age.h", "long"));

	template_free(&linkedlist);
	replacement_free(&ll_age);

	return MUNIT_OK;
}

MunitResult test_generator_linkedlist_vec2(const MunitParameter params[], void *userdata) {
	ctemplate linkedlist = template_create(LINKEDLIST);
	template_addfile       (&linkedlist, "linked_list.htpl", "linked_list_$T.h");
	template_addfile       (&linkedlist, "linked_list.ctpl", "linked_list_$T.c");
	template_addreplacement(&linkedlist, "$T",     NULL);
	template_addreplacement(&linkedlist, "^T",     NULL);
	template_addreplacement(&linkedlist, "PRINTF", "printf");
	template_addreplacement(&linkedlist, "FREE",   "free");
	template_addreplacement(&linkedlist, "CALLOC", "calloc");
	template_addreplacement(&linkedlist, "HEADER", "stdint.h");

	replacement ll_vec2 = replacement_create();
	replacement_add(&ll_vec2, "$T", "vec2");
	replacement_add(&ll_vec2, "^T", "vector2");

	generator_run(settings_custom(.search_paths=paths("templates"), .outdir="output"), linkedlist, ll_vec2);

	munit_assert_file_exists("output/linked_list_vec2.h");
	munit_assert_file_exists("output/linked_list_vec2.c");

	munit_assert_int(util_count_symbols("templates/linked_list.htpl", "$T"), ==, util_count_symbols("output/linked_list_vec2.h", "vec2"));
	munit_assert_int(util_count_symbols("templates/linked_list.htpl", "^T"), ==, util_count_symbols("output/linked_list_vec2.h", "vector2"));

	template_free(&linkedlist);
	replacement_free(&ll_vec2);

	return MUNIT_OK;
}

MunitResult test_generator_queue_vec3(const MunitParameter params[], void *userdata) {
	ctemplate linkedlist = template_create(LINKEDLIST);
	template_addfile       (&linkedlist, "linked_list.htpl", "linked_list_$T.h");
	template_addfile       (&linkedlist, "linked_list.ctpl", "linked_list_$T.c");
	template_addreplacement(&linkedlist, "$T",     NULL);
	template_addreplacement(&linkedlist, "^T",     NULL);
	template_addreplacement(&linkedlist, "PRINTF", "printf");
	template_addreplacement(&linkedlist, "FREE",   "free");
	template_addreplacement(&linkedlist, "CALLOC", "calloc");
	template_addreplacement(&linkedlist, "HEADER", "stdint.h");

	ctemplate queue 		= template_create(QUEUE);
	template_addfile       (&queue, "queue.htpl", "queue_$T.h");
	template_addfile       (&queue, "queue.ctpl", "queue_$T.c");
	template_addreplacement(&queue, "$T",     NULL);
	template_addreplacement(&queue, "^T",     NULL);
	template_addreplacement(&queue, "PRINTF", "printf");
	template_addreplacement(&queue, "FREE",   "free");
	template_addreplacement(&queue, "CALLOC", "calloc");
	template_addreplacement(&queue, "HEADER", "stdint.h");

	forward_table fwd_q_ll = forward_table_create();
	forward_table_forward(&fwd_q_ll, fwd(.symbol="$T", .as="$T"));
	forward_table_forward(&fwd_q_ll, fwd(.symbol="^T", .as="^T"));
	template_adddep(&queue, linkedlist, fwd_q_ll);

	replacement q_vec2 = replacement_create();
	replacement_add(&q_vec2, "$T", "vector3");
	replacement_add(&q_vec2, "^T", "vec3");

	generator_run(settings_custom(.verbose = false, .search_paths=paths("templates"), .outdir="output"), queue, q_vec2);

	munit_assert_file_exists("output/linked_list_vector3.h");
	munit_assert_file_exists("output/linked_list_vector3.c");

	munit_assert_file_exists("output/queue_vector3.h");
	munit_assert_file_exists("output/queue_vector3.c");

	munit_assert_int(util_count_symbols("templates/queue.htpl", "$T"), ==, util_count_symbols("output/queue_vector3.h", "vector3"));
	munit_assert_int(util_count_symbols("templates/queue.htpl", "^T"), ==, util_count_symbols("output/queue_vector3.h", "vec3"));

	munit_assert_int(util_count_symbols("templates/linked_list.htpl", "$T"), ==, util_count_symbols("output/linked_list_vector3.h", "vector3"));
	munit_assert_int(util_count_symbols("templates/linked_list.htpl", "^T"), ==, util_count_symbols("output/linked_list_vector3.h", "vec3"));

	template_free(&linkedlist);
	template_free(&queue);
	forward_table_free(&fwd_q_ll);
	replacement_free(&q_vec2);

	return MUNIT_OK;
}

MunitTest generator_tests[] = {
	{ "/linkedlist_string", test_generator_linkedlist_string, NULL, NULL, MUNIT_TEST_OPTION_NONE, NULL},
	{ "/linkedlist_long",   test_generator_linkedlist_long,   NULL, NULL, MUNIT_TEST_OPTION_NONE, NULL},
	{ "/linkedlist_vec2",   test_generator_linkedlist_vec2,   NULL, NULL, MUNIT_TEST_OPTION_NONE, NULL},
	{ "/queue_vec3",        test_generator_queue_vec3,        NULL, NULL, MUNIT_TEST_OPTION_NONE, NULL},
  { NULL, NULL, NULL, NULL, MUNIT_TEST_OPTION_NONE, NULL }
};

#endif
