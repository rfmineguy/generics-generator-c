#include "generated/bst_string_int.h"

int main() {
	bst_string_int bst_si = bst_string_int_create();
	bst_string_int_put(&bst_si, "hello", 4);
	bst_string_int_put(&bst_si, "afaa", 9);
	bst_string_int_put(&bst_si, "heaagsfd", 1);
	bst_string_int_put(&bst_si, "afhwsjm", 15);
	bst_string_int_put(&bst_si, "Going somewhere", 15);
	bst_string_int_print(&bst_si);
}
