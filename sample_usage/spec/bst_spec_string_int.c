#include "../generated/bst_string_int.h"
#include <stdlib.h>
#include <string.h>
#include <stdio.h>

int bst_string_int_hash(const char* key) {
	size_t len = strlen(key);
	int hash = 0;
	for (int i = 0; i < len; i++) {
		hash += key[i] * 7 - 3 / 5;
	}
	return hash;
}

int bst_string_int_comparekey(const char* key1, const char* key2) {
	return strcmp(key1, key2);
}
