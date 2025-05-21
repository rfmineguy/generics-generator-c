#define GENGEN_IMPLEMENTATION
#include "gengen.h"

int main() {
	ctemplate linkedlist = template_create();
	template_addfile       (&linkedlist, "linked_list.htpl", "linked_list_$T.h");
	template_addfile       (&linkedlist, "linked_list.ctpl", "linked_list_$T.c");
	template_addreplacement(&linkedlist, "$T",     NULL);
	template_addreplacement(&linkedlist, "^T",     NULL);
	template_addreplacement(&linkedlist, "PRINTF", "printf");
	template_addreplacement(&linkedlist, "FREE",   "free");
	template_addreplacement(&linkedlist, "CALLOC", "calloc");
	template_addreplacement(&linkedlist, "HEADER", "stdint.h");

	ctemplate binarytree = template_create();
	template_addfile    	 (&binarytree, "binary_tree.htpl", "binary_tree_^KEY_^VAL.h");
	template_addfile    	 (&binarytree, "binary_tree.ctpl", "binary_tree_^KEY_^VAL.c");
	template_addreplacement(&binarytree, "$KEY",   NULL);
	template_addreplacement(&binarytree, "^KEY",   NULL);
	template_addreplacement(&binarytree, "$VAL",   NULL);
	template_addreplacement(&binarytree, "^VAL",   NULL);
	template_addreplacement(&binarytree, "PRINTF", "printf");
	template_addreplacement(&binarytree, "FREE",   "free");
	template_addreplacement(&binarytree, "CALLOC", "calloc");
	template_addreplacement(&binarytree, "HEADER", "stdint.h");

	ctemplate queue  	  =  template_create();
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

	forward_table fwd_q_bt = forward_table_create();
	forward_table_forward(&fwd_q_bt, fwd(.symbol="$T", .as="$T"));

	replacement ll_int = replacement_create();
	replacement_add(&ll_int, "$T",     "int");
	replacement_add(&ll_int, "^T",     "int");

	replacement ll_string = replacement_create();
	replacement_add(&ll_string, "$T",  "string");
	replacement_add(&ll_string, "^T",  "const char*");

	replacement bt_int = replacement_create();
	replacement_add(&bt_int, "$KEY",   "const char*");
	replacement_add(&bt_int, "^KEY",   "string");
	replacement_add(&bt_int, "$VAL",	 "int");
	replacement_add(&bt_int, "^VAL", 	 "int");

	replacement queue_int = replacement_create();
	replacement_add(&queue_int, "$T",  "int");
	replacement_add(&queue_int, "^T",  "int");

	replacement queue_long = replacement_create();
	replacement_add(&queue_long, "$T",  "long");
	replacement_add(&queue_long, "^T",  "long");

	generator_run(settings_custom(.search_paths=paths("demo_templates", "."), .outdir="."), linkedlist, ll_int);
	generator_run(settings_custom(.search_paths=paths("demo_templates", "."), .outdir="."), linkedlist, ll_string);
	generator_run(settings_custom(.search_paths=paths("demo_templates", "."), .outdir="."), queue     , queue_int);
	generator_run(settings_custom(.search_paths=paths("demo_templates", "."), .outdir="."), queue     , queue_long);

	template_free(&linkedlist);
	template_free(&binarytree);
	template_free(&queue);
	replacement_free(&ll_int);
	replacement_free(&ll_string);
	replacement_free(&bt_int);
	replacement_free(&queue_int);
}

