#define GENGEN_IMPLEMENTATION
#include "gengen2.h"

int main() {
	ctemplate linkedlist = template_create("linkedlist");
	template_addfile       (&linkedlist, "linked_list.htpl", "linked_list_$T.h");
	template_addfile       (&linkedlist, "linked_list.ctpl", "linked_list_$T.c");
	template_addreplacement(&linkedlist, "$T",     NULL);
	template_addreplacement(&linkedlist, "^T",     NULL);
	template_addreplacement(&linkedlist, "PRINTF", "printf");
	template_addreplacement(&linkedlist, "FREE",   "free");
	template_addreplacement(&linkedlist, "CALLOC", "calloc");
	template_addreplacement(&linkedlist, "HEADER", "stdint.h");
	template_addreplacement(&linkedlist, "FDECL",  "stdint.h");

	ctemplate binarytree = template_create("binarytree");
	template_addfile    	 (&binarytree, "bst.htpl", "bst_$KEY_$VAL.h");
	template_addfile    	 (&binarytree, "bst.ctpl", "bst_$KEY_$VAL.c");
	template_addreplacement(&binarytree, "$KEY",   NULL);
	template_addreplacement(&binarytree, "^KEY",   NULL);
	template_addreplacement(&binarytree, "$VAL",   NULL);
	template_addreplacement(&binarytree, "^VAL",   NULL);
	template_addreplacement(&binarytree, "PRINTF", "printf");
	template_addreplacement(&binarytree, "FREE",   "free");
	template_addreplacement(&binarytree, "CALLOC", "calloc");
	template_addreplacement(&binarytree, "HEADER", "stdint.h");

	ctemplate queue  	  =  template_create("queue");
	template_addfile       (&queue, "queue.htpl", "queue_$T.h");
	template_addfile       (&queue, "queue.ctpl", "queue_$T.c");
	template_addreplacement(&queue, "$T",     NULL);
	template_addreplacement(&queue, "^T",     NULL);
	template_addreplacement(&queue, "PRINTF", "printf");
	template_addreplacement(&queue, "FREE",   "free");
	template_addreplacement(&queue, "CALLOC", "calloc");
	template_addreplacement(&queue, "HEADER", "stdint.h");

	forward_table fwd_q_ll = forward_table_create();
	forward_table_forward(&fwd_q_ll, fwd(symbollit("$T"), .as="$T"));
	forward_table_forward(&fwd_q_ll, fwd(symbollit("^T"), .as="^T"));
	template_adddep(&queue, linkedlist, fwd_q_ll, dep_settings(.embed=true, .embedloc=0));

	forward_table fwd_bst_ll = forward_table_create();
	forward_table_forward(&fwd_bst_ll, fwd(symbolfmt("{$KEY}_{$VAL}"), .as="$T"));
	forward_table_forward(&fwd_bst_ll, fwd(symbolfmt("bst_data_node_{$KEY}_{$VAL}"), .as="^T"));
	template_adddep(&binarytree, linkedlist, fwd_bst_ll, dep_settings(.embed=true, .embedloc=0));

	replacement ll_int = replacement_create();
	replacement_add(&ll_int, "$T",     "int");
	replacement_add(&ll_int, "^T",     "int");

	replacement ll_string = replacement_create();
	replacement_add(&ll_string, "$T",  "string");
	replacement_add(&ll_string, "^T",  "const char*");

	replacement bst_string_int = replacement_create();
	replacement_add(&bst_string_int, "$KEY",   "string");
	replacement_add(&bst_string_int, "^KEY",   "const char*");
	replacement_add(&bst_string_int, "$VAL",	 "int");
	replacement_add(&bst_string_int, "^VAL", 	 "int");

	replacement queue_int = replacement_create();
	replacement_add(&queue_int, "$T",  "int");
	replacement_add(&queue_int, "^T",  "int");

	replacement queue_long = replacement_create();
	replacement_add(&queue_long, "$T",  "long");
	generator_run(gen_settings(.verbose = true, .embed_deps = true, .search_paths=paths("demo_templates", "."), .outdir="output"), queue     , queue_int);
	generator_run(gen_settings(.verbose = true, .embed_deps = true, .dryrun = false, .search_paths=paths("demo_templates", "."), .outdir="output"), binarytree, bst_string_int);
	generator_run(gen_settings(.verbose = true, .embed_deps = false, .dryrun = false, .search_paths=paths("demo_templates", "."), .outdir="output"), linkedlist, ll_string);

	template_free(&linkedlist);
	template_free(&binarytree);
	template_free(&queue);
	replacement_free(&ll_int);
	replacement_free(&ll_string);
	replacement_free(&bst_string_int);
	replacement_free(&queue_int);
	replacement_free(&queue_long);
	forward_table_free(&fwd_q_ll);
	forward_table_free(&fwd_bst_ll);
}

