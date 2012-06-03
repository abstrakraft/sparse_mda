# Sparse MDA #

## Description ##
This is an implementation of a sparse multi-dimensional array. The nth index is the key to the nth level of a trie.

## Use ##
Include sparse_mda.h, link against sparse_mda and int_trie. (list is not currently used)

	sparse_mda_t *sparse_mda_init(int dim_count, int size[], double default_value);

Initializes a sparse mda.

- `dim_count` Number of dimensions.
- `size` Array of length <dim_count> that represents the size of the mda in each dimension.
- `default_value` The value returned for any non-specified element.

	int sparse_mda_set(sparse_mda_t *mda, int indices[], double value);

Sets an element of a sparse mda.

- `mda` Initialized sparse mda.
- `indices` Array of integers representing the index to set.
- `value` Value to set.

	double sparse_mda_get(sparse_mda_t *mda, int indices[]);

Retrieve an element of a sparse mda.  If the element has been set, returns the set value, otherwise, returns the default value.

- `mda` Initialized sparse mda.
- `indices`Array of integers representing the index to get.

## Future Work ##
Additional API functions (free).  Python and Matlab wrappers.
