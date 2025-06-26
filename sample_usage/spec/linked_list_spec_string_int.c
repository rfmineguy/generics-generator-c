#include "../generated/bst_string_int.h"
#include <stdio.h>

void ll_string_int_custom_free(ll_node_string_int* n) {
	// nothing to free
}

void ll_string_int_print_node(ll_node_string_int* n) {
	printf("(%s, %d)", n->val.key, n->val.val);
}
