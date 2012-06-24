#include <stdlib.h>
#include "avl_tree.h"

avl_tree_t *
avl_tree_init(cmp_t *cmp)
{
	avl_tree_t *tree;
	if (!(tree = malloc(sizeof *tree))) {
		return NULL;
	}

	tree->root = NULL;
	tree->cmp = cmp;

	return tree;
}

int
avl_tree_insert(avl_tree_t *tree, void *data)
{
	avl_node_t *node;
	if (!(node = avl_node_init(data))) {
		return -1;
	}

	if (tree->root) {
		return avl_node_insert(&tree->root, tree->cmp, node);
	}
	else {
		tree->root = node;
		return 0;
	}
}

int
avl_tree_delete(avl_tree_t *tree, void *data)
{
	if (tree->root) {
		return avl_node_delete(&tree->root, tree->cmp, data);
	}
	else {
		return -1;
	}
}

void *
avl_tree_search(avl_tree_t *tree, void *data)
{
	if (tree->root) {
		return avl_node_search(tree->root, tree->cmp, data);
	}
	else {
		return NULL;
	}
}

char *
avl_tree_str(avl_tree_t *tree, str_t *str)
{
	if (tree->root) {
		return avl_node_str(tree->root, str);
	}
	else {
		return "";
	}
}

void
avl_tree_free(avl_tree_t *tree)
{
	if (tree->root) {
		avl_node_free(tree->root, 1);
	}

	free(tree);
}

avl_iter_t *
avl_iter_init(avl_tree_t *tree)
{
	avl_iter_t *iter;

	if (!(iter = malloc(sizeof *iter))) {
		return NULL;
	}

	if (tree->root) {
		*iter = tree->root;
		while ((*iter)->child[0]) {
			*iter = (*iter)->child[0];
		}
	}
	else {
		*iter = NULL;
	}

	return iter;
}

void
avl_iter_next(avl_iter_t *iter)
{
	avl_node_t *prev;

	if ((*iter)->child[1]) {
		*iter = (*iter)->child[1];
		while ((*iter)->child[0]) {
			*iter = (*iter)->child[0];
		}
	}
	else {
		do {
			prev = *iter;
			*iter = (*iter)->parent;
		} while ((*iter) && ((*iter)->child[1] == prev));
	}
}

void *
avl_iter_get(avl_iter_t *iter)
{
	if (*iter) {
		return (*iter)->data;
	}
	else {
		return NULL;
	}
}

void
avl_iter_free(avl_iter_t *iter)
{
	free(iter);
}
