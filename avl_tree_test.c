#include <stdio.h>
#include <stdlib.h>
#include <limits.h>
#include "avl_tree.h"

#define RAND_TEST 1
#define N 1000000

int
cmp(int *v1, int *v2)
{
	if (*v1 > *v2) {
		return 1;
	}
	else if (*v1 == *v2) {
		return 0;
	}
	else {
		return -1;
	}
}

char *
node2s(avl_node_t *node)
{
	int len;
	char *s;

	len = snprintf(NULL, 0, "%d(%d)", *((int *)node->data), node->balance)+1;
	if (!(s = malloc(len))) {
		return NULL;
	}

	snprintf(s, len, "%d(%d)", *((int *)node->data), node->balance);

	return s;
}

int
iter_list(avl_tree_t *tree, int print)
{
	avl_iter_t *iter;
	void *data;
	int prev = INT_MIN;

	if (!(iter = avl_iter_init(tree))) {
		fprintf(stderr, "error on avl_iter_init\n");
		return -1;
	}

	while (data = avl_iter_get(iter)) {
		if (prev > *(int *)data) {
			fprintf(stderr, "Out of order list\n");
			return -1;
		}
		if (print) {
			printf("%d ", *(int *)data);
		}
		avl_iter_next(iter);
	}
	if (print) {
		printf("\n");
	}

	return 0;
}

#if RAND_TEST
int test_ints[N];
#else
int test_ints[] = {1,-1,2,-2,3,4,5,6,7,8,9,10};
#endif

int
main(int argc, char *argv[])
{
	int idx;
	avl_tree_t *tree;

	if (!(tree = avl_tree_init((cmp_t *)cmp))) {
		fprintf(stderr, "error on avl_tree_init\n");
		return -1;
	}

	for (idx = 0; idx < sizeof(test_ints)/sizeof(test_ints[0]); idx++) {
#if RAND_TEST
		test_ints[idx] = rand();
		if (test_ints[idx] == 0) {
			test_ints[idx]++;
		}
#endif
		if (avl_tree_insert(tree, &test_ints[idx]) < 0) {
			fprintf(stderr, "error on avl_tree_insert\n");
			return -1;
		}
	}

	/* printf("%s", avl_tree_str(tree, node2s)); */
	if (iter_list(tree, !RAND_TEST)) {
		return -1;
	}
	/* printf("\n"); */

#if RAND_TEST
	for (idx = 0; idx < sizeof(test_ints)/sizeof(test_ints[0])/2; idx++) {
		avl_tree_delete(tree, &test_ints[idx]);
	}
#else
	avl_tree_delete(tree, &test_ints[6]);
	avl_tree_delete(tree, &test_ints[2]);
	avl_tree_delete(tree, &test_ints[1]);
	avl_tree_delete(tree, &test_ints[9]);
	avl_tree_delete(tree, &test_ints[11]);
	avl_tree_delete(tree, &test_ints[4]);
#endif

	/* printf("%s", avl_tree_str(tree, node2s)); */
	if (iter_list(tree, !RAND_TEST)) {
		return -1;
	}

	printf("Test successful!\n");
	return 0;
}
