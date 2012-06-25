#include <stdlib.h>
#include <string.h>
#include "sparse_mda.h"

typedef struct data data_t;
struct data {
	int idx;
	union {
		double value;
		avl_tree_t *tree;
	} payload;
};

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

sparse_mda_t *
sparse_mda_init(int dim_count, double defval)
{
	int idx;
	sparse_mda_t *m;

	if (dim_count < 1) {
		return NULL;
	}

	if (!(m = malloc(sizeof *m))) {
		return NULL;
	}

	m->dim_count = dim_count;
	m->defval = defval;

	if (!(m->tree = avl_tree_init((cmp_t *)cmp))) {
		free(m);
		return NULL;
	}

	return m;
}

int
sparse_mda_set(sparse_mda_t *m, int indices[], double value)
{
	int idx;
	double *val_ptr;
	data_t *data;
	avl_tree_t *cursor;

	cursor = m->tree;
	for (idx = 0; idx < m->dim_count; idx++) {
		data = avl_tree_search(cursor, &indices[idx]);
		if (!data) {
			if (!(data = malloc(sizeof data))) {
				return -1;
			}
			data->idx = indices[idx];
			/* If this is the last dim, set the value */
			if (idx == m->dim_count-1) {
				data->payload.value = value;
			}
			else { /* otherwise, create a new tree */
				if (!(data->payload.tree = avl_tree_init((cmp_t *)cmp))) {
					free(data);
					return -1;
				}
			}
			if (avl_tree_insert(cursor, data)) {
				free(data);
				return -1;
			}
		}
		/* this is garbage on the last dimension */
		cursor = data->payload.tree;
	}

	return 0;
}

double
sparse_mda_get(sparse_mda_t *m, int indices[])
{
	int idx;
	data_t *data;
	avl_tree_t *cursor;

	/* Get containing trie */
	cursor = m->tree;
	for (idx = 0; idx < m->dim_count; idx++) {
		data = avl_tree_search(cursor, &indices[idx]);
		if (!data) {
			return m->defval;
		}
		/* this is garbage on the last dimension */
		cursor = data->payload.tree;
	}

	return data->payload.value;
}
