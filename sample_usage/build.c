#define GENGEN_IMPLEMENTATION
#include "../gengen2.h"

int main() {
	ctemplate linkedlist = template_create("linkedlist");
	template_addfile       (&linkedlist, "linked_list.htpl", "linked_list_$T.h");
	template_addfile       (&linkedlist, "linked_list.ctpl", "linked_list_$T.c");
	template_addreplacement(&linkedlist, "$T",     NULL);
	template_addreplacement(&linkedlist, "^T",     NULL);
	template_addreplacement(&linkedlist, "PRINT", "printf");
	template_addreplacement(&linkedlist, "FREE",   "free");
	template_addreplacement(&linkedlist, "CALLOC", "calloc");
	template_addreplacement(&linkedlist, "HEADER", "stdint.h");

	ctemplate binarytree = template_create("binarytree");
	template_addfile    	 (&binarytree, "bst.htpl", "bst_$KEY_$VAL.h");
	template_addfile    	 (&binarytree, "bst.ctpl", "bst_$KEY_$VAL.c");
	template_addreplacement(&binarytree, "$KEY",   NULL);
	template_addreplacement(&binarytree, "^KEY",   NULL);
	template_addreplacement(&binarytree, "$VAL",   NULL);
	template_addreplacement(&binarytree, "^VAL",   NULL);
	template_addreplacement(&binarytree, "PRINT", "printf");
	template_addreplacement(&binarytree, "FREE",   "free");
	template_addreplacement(&binarytree, "CALLOC", "calloc");
	template_addreplacement(&binarytree, "HEADER", "stdint.h");

	forward_table fwd_bst_ll = forward_table_create();
	forward_table_forward(&fwd_bst_ll, fwd(symbolfmt("{$KEY}_{$VAL}"), .as="$T"));
	forward_table_forward(&fwd_bst_ll, fwd(symbolfmt("bst_data_node_{$KEY}_{$VAL}"), .as="^T"));
	template_adddep(&binarytree, linkedlist, fwd_bst_ll, dep_settings(.embed=true));

	replacement bst_string_int = replacement_create();
	replacement_add(&bst_string_int, "$KEY",   "string");
	replacement_add(&bst_string_int, "^KEY",   "const char*");
	replacement_add(&bst_string_int, "$VAL",	 "int");
	replacement_add(&bst_string_int, "^VAL", 	 "int");

	generator_run(gen_settings(.embed_deps=true, .search_paths=paths("../demo_templates", "."), .outdir="generated"), binarytree, bst_string_int);

	template_free(&linkedlist);
	template_free(&binarytree);
	replacement_free(&bst_string_int);
	forward_table_free(&fwd_bst_ll);
}

