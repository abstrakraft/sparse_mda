#include <stdio.h>
#include <stdlib.h>
#include "int_trie.h"

int_trie_t *
int_trie_init(int max_children)
{
	int idx;
	int_trie_t *trie;

	if (!(trie = malloc(sizeof *trie))) {
		return NULL;
	}

	if (!(trie->children = malloc(max_children*(sizeof trie->children)))) {
		free(trie);
		return NULL;
	}

	for (idx = 0; idx < max_children; idx++) {
		trie->children[idx] = NULL;
	}

	return trie;
}

void
int_trie_set(int_trie_t *trie, int key, void *val)
{
	trie->children[key] = val;
}

void *
int_trie_get(int_trie_t *trie, int key)
{
	return trie->children[key];
}
