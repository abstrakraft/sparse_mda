#ifndef SPARSE_MDA_H

#include "int_trie.h"

typedef struct sparse_mda sparse_mda_t;
struct sparse_mda {
	int_trie_t *trie;
	int *size;
	int dim_count;
	double defval;
};

sparse_mda_t *sparse_mda_init(int, int [], double);
int sparse_mda_set(sparse_mda_t *, int [], double);
double sparse_mda_get(sparse_mda_t *, int []);

#endif SPARSE_MDA_H
