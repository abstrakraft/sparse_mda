#ifndef AVL_TREE_H

typedef struct avl_tree avl_tree_t;
typedef struct avl_node avl_node_t;
typedef avl_node_t *avl_iter_t;

typedef int cmp_t(const void *, const void *);
typedef char *str_t(avl_node_t *);

struct avl_tree {
	avl_node_t *root;
	cmp_t *cmp;
};

struct avl_node {
	avl_node_t *parent;
	avl_node_t *child[2];
	void *data;
	int balance;
};

avl_tree_t *avl_tree_init(cmp_t *);
int avl_tree_insert(avl_tree_t *, void *);
int avl_tree_delete(avl_tree_t *, void *);
void *avl_tree_search(avl_tree_t *, void *);
char *avl_tree_str(avl_tree_t *, str_t *);
void avl_tree_free(avl_tree_t *);

avl_iter_t *avl_iter_init(avl_tree_t *);
void avl_iter_next(avl_iter_t *);
void *avl_iter_get(avl_iter_t *);
void avl_iter_free(avl_iter_t *);

avl_node_t *avl_node_init(void *);
int avl_node_insert(avl_node_t **, cmp_t *, avl_node_t *);
int avl_node_delete(avl_node_t **, cmp_t *, void *);
void *avl_node_search(avl_node_t *, cmp_t *, void *);
char *avl_node_str(avl_node_t *, str_t *);
int avl_node_rebalance(avl_node_t **);
int avl_node_pick_rightmost(avl_node_t **, avl_node_t **);
void avl_node_free(avl_node_t *, int);

#endif
