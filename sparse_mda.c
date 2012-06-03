#include <stdlib.h>
#include <string.h>
#include "sparse_mda.h"

sparse_mda_t *
sparse_mda_init(int dim_count, int size[], double defval)
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

	if (!(m->size = malloc(dim_count * sizeof(int)))) {
		free(m);
		return NULL;
	}
	memcpy(m->size, size, dim_count * sizeof(int));

	if (!(m->trie = int_trie_init(size[0]))) {
		free(m->size);
		free(m);
		return NULL;
	}

	return m;
}

int
sparse_mda_set(sparse_mda_t *m, int indices[], double val)
{
	int idx;
	double *val_ptr;
	int_trie_t *cursor, *prev_cursor;

	/* Get containing trie */
	cursor = m->trie;
	for (idx = 0; idx < m->dim_count-1; idx++) {
		prev_cursor = cursor;
		cursor = int_trie_get(prev_cursor, indices[idx]);
		if (!cursor) {
			if (!(cursor = int_trie_init(m->size[idx+1]))) {
				return -1;
			}
			int_trie_set(prev_cursor, indices[idx], cursor);
		}
	}

	/* Get pointer to value and set */
	val_ptr = int_trie_get(cursor, indices[m->dim_count-1]);
	if (!val_ptr) {
		if (!(val_ptr = malloc(sizeof(double)))) {
			return -1;
		}
		int_trie_set(cursor, indices[m->dim_count-1], val_ptr);
	}
	*val_ptr = val;

	return 0;
}

double
sparse_mda_get(sparse_mda_t *m, int indices[])
{
	int idx;
	double *val_ptr;
	int_trie_t *cursor, *prev_cursor;

	/* Get containing trie */
	cursor = m->trie;
	for (idx = 0; idx < m->dim_count-1; idx++) {
		cursor = int_trie_get(cursor, indices[idx]);
		if (!cursor) {
			return m->defval;
		}
	}

	/* Get pointer to value */
	val_ptr = int_trie_get(cursor, indices[m->dim_count-1]);
	if (!val_ptr) {
		return m->defval;
	}
	return *val_ptr;
}
