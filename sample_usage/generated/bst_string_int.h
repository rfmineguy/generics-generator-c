#ifndef BIN_SEARCH_TREE_string_int_H
#define BIN_SEARCH_TREE_string_int_H
#include <stdbool.h>

typedef struct bst_data_node_string_int {
	const char* key;
	int val;
} bst_data_node_string_int;

#ifndef LINKED_LIST_string_int_H
#define LINKED_LIST_string_int_H

typedef struct ll_node_string_int {
	struct ll_node_string_int *next, *prev;
	bst_data_node_string_int val;
} ll_node_string_int;

typedef struct ll_string_int {
	ll_node_string_int *head, *tail;
} ll_string_int;

ll_string_int ll_string_int_create();
void  ll_string_int_free(ll_string_int*);
void  ll_string_int_pushback(ll_string_int*, bst_data_node_string_int);
void  ll_string_int_popback(ll_string_int*);
bst_data_node_string_int*   ll_string_int_peekback(ll_string_int*);
void  ll_string_int_print(ll_string_int*);

#endif

typedef struct bst_string_int {
	ll_string_int buckets[10];
} bst_string_int;

bst_string_int   bst_string_int_create();
void            bst_string_int_put(bst_string_int* bst, const char* key, int val);
int*           bst_string_int_getptr(bst_string_int* bst, const char* key);
bool            bst_string_int_contains(bst_string_int* bst, const char* key);
void 						bst_string_int_print(bst_string_int* bst);

#endif
