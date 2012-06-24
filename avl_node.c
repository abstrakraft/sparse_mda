#include <stdlib.h>
#include <string.h>
#include "avl_tree.h"

avl_node_t *
avl_node_init(void *data)
{
	avl_node_t *node;
	if (!(node = malloc(sizeof *node))) {
		return NULL;
	}

	node->parent = node->child[0] = node->child[1] = NULL;
	node->data = data;
	node->balance = 0;

	return node;
}

int
avl_node_insert(avl_node_t **p_node, cmp_t *cmp, avl_node_t *node)
{
	int c = cmp(node->data, (*p_node)->data);
	int idx = (c>=0) ? 1 :  0;
	int sgn = (c>=0) ? 1 : -1;

	if ((*p_node)->child[idx]) {
		int child_grew, shrank;

		child_grew = avl_node_insert(&(*p_node)->child[idx], cmp, node);
		(*p_node)->balance += child_grew * sgn;
		shrank = avl_node_rebalance(p_node);
		/* In the insert case, shrank is always true IF the tree was rebalanced
		   If the tree was not rebalanced (!shrank), and self.balance != 0
		   AND the child grew, then this tree also grew */
		return child_grew && !shrank && (*p_node)->balance;
	}
	else {
		(*p_node)->child[idx] = node;
		node->parent = *p_node;
		(*p_node)->balance += sgn;
		return abs((*p_node)->balance);
	}
}

int avl_node_delete(avl_node_t **p_node, cmp_t *cmp, void *data)
{
	int c, child_shrank;
	avl_node_t *node, *picked;

	node = *p_node;
	c = cmp(data, node->data);
	if (c == 0) {
		if (node->child[0] == NULL) {
			*p_node = node->child[1];
			if (node->child[1]) {
				node->child[1]->parent = node->parent;
			}
			avl_node_free(node, 0);
			return 1;
		}
		else if (node->child[1] == NULL) {
			*p_node = node->child[0];
			if (node->child[0]) {
				node->child[0]->parent = node->parent;
			}
			avl_node_free(node, 0);
			return 1;
		}
		else {
			child_shrank = avl_node_pick_rightmost(&node->child[0], &picked);
			picked->child[0] = node->child[0];
			if (picked->child[0]) {
				picked->child[0]->parent = picked;
			}
			picked->child[1] = node->child[1];
			if (picked->child[1]) {
				picked->child[1]->parent = picked;
			}
			picked->child[1]->parent = picked;
			picked->parent = node->parent;
			picked->balance = node->balance + child_shrank;
			avl_node_free(node, 0);
			*p_node = picked;
			node = *p_node;
		}
	}
	else {
		int idx = (c>0) ? 1 :  0;
		int sgn = (c>0) ? 1 : -1;

		if (node->child[idx]) {
			if ((child_shrank = avl_node_delete(&node->child[idx], cmp, data)) < 0) {
				/* error condition */
				return child_shrank;
			}
			node->balance -= child_shrank * sgn;
		}
		else {
			return -1;
		}
	}

	if (child_shrank) {
		if (node->balance == 0) {
			return 1;
		}
		else {
			return avl_node_rebalance(p_node);
		}
	}
	else {
		return 0;
	}
}

int
avl_node_pick_rightmost(avl_node_t **p_node, avl_node_t **picked)
{
	avl_node_t *node = *p_node;

	if (node->child[1]) {
		int child_shrank = avl_node_pick_rightmost(&node->child[1], picked);
		node->balance -= child_shrank;
		if (child_shrank) {
			if (node->balance == 0) {
				return 1;
			}
			else {
				return avl_node_rebalance(p_node);
			}
		}
		else {
			return 0;
		}
	}
	else {
		*picked = node;
		*p_node = node->child[0];
		if (node->child[0]) {
			node->child[0]->parent = node->parent;
		}
		return 1;
	}
}

void *
avl_node_search(avl_node_t *node, cmp_t *cmp, void *data)
{
	avl_node_t *cursor = node;

	do {
		int c = cmp(data, node->data);
		if (c == 0) {
			return node->data;
		}
		else {
			int idx = (c>=0) ? 1 :  0;
			cursor = cursor->child[idx];
		}
	} while (cursor);

	return NULL;
}

void
avl_node_rot(avl_node_t **p_node, char dir)
{
	int d_idx, c_idx;
	avl_node_t *tmp;

	d_idx = (dir>0) ? 1 : 0; /* index of rotation direction */
	c_idx = (dir>0) ? 0 : 1; /* index of complimentary direction */

	tmp = (*p_node)->child[c_idx];
	tmp->parent = (*p_node)->parent;

	(*p_node)->child[c_idx] = tmp->child[d_idx];
	if ((*p_node)->child[c_idx]) {
		(*p_node)->child[c_idx]->parent = *p_node;
	}

	tmp->child[d_idx] = *p_node;
	(*p_node)->parent = tmp;

	*p_node = tmp;
}

int
avl_node_rebalance(avl_node_t **p_node)
{
	avl_node_t *node = *p_node;
	if (abs(node->balance) < 2) {
		return 0;
	}
	else {
		int sgn = (node->balance == 2) ? 1 : -1;
		int idx1 = (node->balance == 2) ? 1 : 0;
		avl_node_t *child1 = node->child[idx1];
		int shrank = child1->balance ? 1 : 0;

		if (child1->balance == sgn) {
			node->balance = 0;
			child1->balance = 0;
		}
		else if (child1->balance == 0) {
			node->balance = sgn;
			child1->balance = -sgn;
		}
		else /* if child1->balance == -sgn */ {
			int idx2 = abs(idx1-1);
			avl_node_t *child2 = child1->child[idx2];

			if (child2->balance == sgn) {
				node->balance = -sgn;
				child1->balance = 0;
			}
			else if (child2->balance == 0) {
				node->balance = 0;
				child1->balance = 0;
			}
			else /* if child2->balance == -sgn */ {
				node->balance = 0;
				child1->balance = sgn;
			}
			child2->balance = 0;
			avl_node_rot(&node->child[idx1], sgn);
		}
		avl_node_rot(p_node, -sgn);
		return shrank;
	}
}

/* This is pretty inefficient right now - a LOT of string copying.
   A better way would be to recursively find the appropriate lengths,
   allocate the string, then recursively fill it in.
   The method below works for now.  For the most part, generating the
   string is only really useful for small tables, so it doesn't matter
   all that much */
char *
avl_node_str(avl_node_t *node, str_t *str)
{
	int idx, jdx;
	int ret_len, node_len, left_len, right_len;
	int ret_height, left_height, right_height;
	char *ret_str, *node_str, *left_str, *right_str;
	char *cursor;

	node_str = str(node);
	node_len = strlen(node_str);

	if (node->child[0]) {
		left_str = avl_node_str(node->child[0], str);
		left_len = strchr(left_str, '\n') - left_str;
		left_height = strlen(left_str) / (left_len+1);
	}
	else {
		left_str = NULL;
		left_len = 1;
		left_height = 0;
	}

	if (node->child[1]) {
		right_str = avl_node_str(node->child[1], str);
		right_len = strchr(right_str, '\n') - right_str;
		right_height = strlen(right_str) / (right_len+1);
	}
	else {
		right_str = NULL;
		right_len = 1;
		right_height = 0;
	}

	ret_len = left_len + node_len + right_len;
	ret_height = 1 + ((left_height > right_height) ? left_height : right_height);
	if (!(ret_str = malloc((ret_len+1)*ret_height+1))) {
		return NULL;
	}

	cursor = ret_str;
	for (idx = 0; idx < left_len; idx++) {
		*cursor++ = ' ';
	}
	strncpy(cursor, node_str, node_len);
	cursor += node_len;
	for (idx = 0; idx < right_len; idx++) {
		*cursor++ = ' ';
	}
	*cursor++ = '\n';

	for (idx = 0; idx < (ret_height-1); idx++) {
		if (idx < left_height) {
			strncpy(cursor, &left_str[(left_len+1)*idx], left_len);
			cursor += left_len;
		}
		else {
			for (jdx = 0; jdx < left_len; jdx++) {
				*cursor++ = ' ';
			}
		}

		for (jdx = 0; jdx < node_len; jdx++) {
			*cursor++ = ' ';
		}

		if (idx < right_height) {
			strncpy(cursor, &right_str[(right_len+1)*idx], right_len);
			cursor += right_len;
		}
		else {
			for (jdx = 0; jdx < right_len; jdx++) {
				*cursor++ = ' ';
			}
		}
		*cursor++ = '\n';
	}
	*cursor++ = '\0';

	free(node_str);
	if (left_str) free(left_str);
	if (right_str) free(right_str);

	return ret_str;
}

void
avl_node_free(avl_node_t *node, int recurse)
{
	if (recurse && node->child[0]) {
		avl_node_free(node->child[0], 1);
	}

	if (recurse && node->child[1]) {
		avl_node_free(node->child[1], 1);
	}

	free(node);
}
