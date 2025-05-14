#define GENGEN_IMPLEMENTATION
#include "gengen.h"

int main() {
	ctemplate linkedlist = template_create("linkedlist");
	template_addfile    (&linkedlist, "linked_list.htpl", "linked_list_$T.h");
	template_addfile    (&linkedlist, "linked_list.ctpl", "linked_list_$T.c");

	ctemplate binarytree = template_create("binarytree");
	template_addfile    (&binarytree, "binary_tree.htpl", "binary_tree_^KEY_^VAL.h");
	template_addfile    (&binarytree, "binary_tree.ctpl", "binary_tree_^KEY_^VAL.c");

	ctemplate queue  	  = template_create("queue");
	template_addfile    (&queue, "queue.htpl", "queue_^T.h");
	template_addfile    (&queue, "queue.ctpl", "queue_^T.c");
	template_adddep 		(&queue, linkedlist);

	replacement ll_int = replacement_create();
	replacement_add(&ll_int, "$T",     "int");
	replacement_add(&ll_int, "^T",     "int");
	replacement_add(&ll_int, "PRINTF", "printf");
	replacement_add(&ll_int, "FREE",   "free");
	replacement_add(&ll_int, "CALLOC", "calloc");
	replacement_add(&ll_int, "HEADER", "stdint.h");

	replacement ll_string = replacement_create();
	replacement_add(&ll_string, "$T",     "string");
	replacement_add(&ll_string, "^T",     "const char*");
	replacement_add(&ll_string, "PRINTF", "printf");
	replacement_add(&ll_string, "FREE",   "free");
	replacement_add(&ll_string, "CALLOC", "calloc");
	replacement_add(&ll_string, "HEADER", "stdint.h");

	replacement bt_int = replacement_create();
	replacement_add(&bt_int, "$KEY",   "const char*");
	replacement_add(&bt_int, "^KEY",   "string");
	replacement_add(&bt_int, "$VAL",	 "int");
	replacement_add(&bt_int, "^VAL", 	 "int");
	replacement_add(&bt_int, "PRINTF", "printf");
	replacement_add(&bt_int, "FREE",   "free");
	replacement_add(&bt_int, "CALLOC", "calloc");
	replacement_add(&bt_int, "HEADER", "stdint.h");

	replacement queue_int = replacement_create();
	replacement_add(&queue_int, "$T",     "int");
	replacement_add(&queue_int, "^T",     "int");
	replacement_add(&queue_int, "PRINTF", "printf");
	replacement_add(&queue_int, "FREE",   "free");
	replacement_add(&queue_int, "CALLOC", "calloc");
	replacement_add(&queue_int, "HEADER", "stdint.h");

	generator_run(settings_custom(.search_paths=paths("demo_templates", "."), .outdir="."), linkedlist, ll_int);
	generator_run(settings_custom(.search_paths=paths("demo_templates", "."), .outdir="."), linkedlist, ll_string);
	generator_run(settings_custom(.search_paths=paths("demo_templates", "."), .outdir="."), binarytree, bt_int);
	generator_run(settings_custom(.search_paths=paths("demo_templates", "."), .outdir="."), queue,  		queue_int);
}
