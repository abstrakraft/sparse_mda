#include <stdio.h>
#include "int_trie.h"

#define MAX_CHILDREN 5
#define INDEX_COUNT 3
#define COST_COUNT 3
int indices[COST_COUNT][INDEX_COUNT] = {{0,1,2},
										{0,2,4},
										{1,2,3}};

int marker;
void
find_marker(int_trie_t *trie, int level)
{
	static int path[INDEX_COUNT];
	void *value;
	int idx, jdx;

	for (idx = 0; idx < MAX_CHILDREN; idx++) {
		path[level] = idx;
		value = int_trie_get(trie, idx);
		if (value) {
			if (level < INDEX_COUNT-1) {
				find_marker((int_trie_t *)value, level+1);
			}
			else {
				for (jdx = 0; jdx < INDEX_COUNT; jdx++) {
					printf("%d ", path[jdx]);
				}
				printf("\n");
			}
		}
	}
}

main(int argc, char *argv[])
{
	int idx, jdx;
	int_trie_t *root, *cursor, *prev_cursor;

	/* Setup */
	if (!(root = int_trie_init(MAX_CHILDREN))) {
		fprintf(stderr, "int_trie_init error\n");
		return -1;
	}

	for (idx = 0; idx < COST_COUNT; idx++) {
		cursor = root;
		for (jdx = 0; jdx < INDEX_COUNT-1; jdx++) {
			prev_cursor = cursor;
			cursor = int_trie_get(prev_cursor, indices[idx][jdx]);
			if (!cursor) {
				if (!(cursor = int_trie_init(MAX_CHILDREN))) {
					fprintf(stderr, "int_trie_init error\n");
					return -1;
				}
				int_trie_set(prev_cursor, indices[idx][jdx], cursor);
			}
		}
		int_trie_set(cursor, indices[idx][INDEX_COUNT-1], &marker);
	}

	/* Test */
	find_marker(root, 0);

	return 0;
}
