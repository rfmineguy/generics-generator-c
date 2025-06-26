#include "bst_string_int.h"
#include <assert.h>

#include "../spec/linked_list_spec_string_int.c"
#include <stdlib.h>

/*
 * ll_string_int_custom_free(ll_node_string_int);
 * 		- this function is required to be implemented in spec file per type
 */

ll_string_int ll_string_int_create() {
	return (ll_string_int) {.head = 0, .tail = 0};
}
 
void  ll_string_int_free(ll_string_int* ll) {
	ll_node_string_int* n = ll->head;
	while (n) {
		ll_node_string_int** t = &n;
		n = n->next;
		ll_string_int_custom_free(*t);
		free(*t);
		*t = 0;
	}
}

void  ll_string_int_pushback(ll_string_int* ll, bst_data_node_string_int val) {
	ll_node_string_int * n = calloc(1, sizeof(ll_node_string_int));
	assert(n && "Pushback allocation failed");
	n->val = val;
	if (ll->head == 0 && ll->tail == 0) {
		ll->head = n;
		ll->tail = ll->head;
		return;
	}
	else {
		ll->tail->next = n;
		n->prev = ll->tail;
		ll->tail = n;
	}
}

void  ll_string_int_popback(ll_string_int* ll) {
	if (ll->head == 0) return;
	ll_node_string_int* n = ll->tail;
	ll->tail = ll->tail->prev;
	ll_string_int_custom_free(n);
	free(n);
}

bst_data_node_string_int*   ll_string_int_peekback(ll_string_int* ll) {
	if (ll->head == 0) return 0;
	return &ll->tail->val;
}

void  ll_string_int_print(ll_string_int* ll) {
	ll_node_string_int* n = ll->head;
	while (n) {
		ll_string_int_print_node(n);
		printf(", ");
		n = n->next;
	}
}
#include "../spec/bst_spec_string_int.c"

/*
 * int bst_string_int_hash(const char* key);
 *    - generate a hash index for 'key'
 */

bst_string_int bst_string_int_create() {
	return (bst_string_int) {0};
}
void bst_string_int_put(bst_string_int* bst, const char* key, int val) {
	int hash = bst_string_int_hash(key) % 10;
	ll_string_int* ll = &bst->buckets[hash];
	ll_string_int_pushback(ll, (bst_data_node_string_int){.key = key, .val = val});
}

int* bst_string_int_getptr(bst_string_int* bst, const char* key) {
	int hash = bst_string_int_hash(key) % 10;
	ll_string_int* ll = &bst->buckets[hash];
	ll_node_string_int* n = ll->head;
	while (n) {
		if (bst_string_int_comparekey(n->val.key, key) == 0) {
			return &n->val.val;
		}
		n = n->next;
	}
	return 0;
}

bool bst_string_int_contains(bst_string_int* bst, const char* key) {
	return bst_string_int_getptr(bst, key) != 0;
}

void bst_string_int_print(bst_string_int* bst) {
	for (int i = 0; i < 10; i++) {
		printf("%d: ", i);
		ll_string_int_print(&bst->buckets[i]);
		printf("\n");
	}
}
