#ifndef SPARSE_MDA_H

#include "avl_tree.h"

typedef struct sparse_mda sparse_mda_t;
struct sparse_mda {
	avl_tree_t *tree;
	int dim_count;
	double defval;
};

sparse_mda_t *sparse_mda_init(int, double);
int sparse_mda_set(sparse_mda_t *, int [], double);
double sparse_mda_get(sparse_mda_t *, int []);

#endif
