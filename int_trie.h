#ifndef INT_TRIE_H

typedef struct int_trie int_trie_t;
struct int_trie {
	void **children;
};

int_trie_t *int_trie_init(int);
void int_trie_free(int_trie_t *);
void int_trie_set(int_trie_t *, int, void *);
void *int_trie_get(int_trie_t *, int);

#endif
