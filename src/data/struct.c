
#include <stdlib.h> /* malloc, exit */
#include <stdio.h> /* printf */

#ifdef DEBUG
#include <assert.h> /* assert */
#endif

#include "struct.h"

/* ===== DATAS ===== */

struct symbol symtab[NHASH];    /* symbol table itself */

/* === FUNCTIONS === */

/**
 * Allocates new memory for the struct and then populates its values.
 * Assigns prev to next without checking because it is either NULL, meaning a new chain or it
 * is a new head, so it becomes the next element.
 * Returns NULL if memory could not be allocated.
 */
struct value *newValue(int i, struct value *prev){
	struct value *a = malloc(sizeof(struct value));
	if (!a){
		printf("out of space");
		return NULL;
	}

	a->i = i;
	a->next = prev;
	#ifdef DEBUG
	assert(a && a->i == i && a->next == prev);
	#endif
	return a;
}

/**
 * Iteratively creates new value elements connected to one another. Finally, the chain is reversed.
 */
struct value *dupValue(struct value *base){
	struct value *head = NULL, *t = NULL;

	for(t = base; t; t = t->next){
		head = newValue(t->i, head);
	}

	reverseValue( &head );

	#ifdef DEBUG
	struct value *h = NULL;
	for(t = base, h = head; t || h; t = t->next, h = h->next){
		assert(t->i == h->i);
	}
	#endif

	return head;
}

/**
 * Iteratively work through the chain until the next element is NULL.
 * Aliases the final element.
 */
struct value *backValue(struct value *base){
	struct value *t = NULL;

	for(t = base; t->next; t = t->next){
	}

	#ifdef DEBUG
	assert(t && t->i && !t->next);
	#endif
	return t;
}

/**
 * Iteratively search for the key and return the first match. Will return NULL
 * if the key is not in base or base itself is NULL.
 * Aliases the found element.
 */
struct value *findValue(int key, struct value *base){
	if (!base) return NULL;
	struct value *t = NULL;

	for(t = base; t; t = t->next){
		if (t->i == key) return t;
	}
	#ifdef DEBUG
	assert(!hasValue(key, base));
	#endif
	return NULL;
}

/**
 * When found, the value is extracted by modifying the previous element's next attribute.
 * The pointer attached to the new value is then returned.
 */
struct value *removeValue(int key, struct value **base){
	if (!*base) return NULL;
	struct value *t = NULL, *prev = NULL;

	#ifdef DEBUG
	int size = countValue(*base);
	#endif

	for(t = *base; t; t = t->next){
		if (t->i == key){
			if (prev)	prev->next 	= t->next; 	/* skip over current */
			else			*base 			= t->next; 	/* set front to next */
			t->next = NULL; 									/* erase link */

			#ifdef DEBUG
			assert(!hasValueExact(t, *base));
			assert(size - 1 == countValue(*base));
			#endif
			return t;
		}

		prev = t;
	}

	#ifdef DEBUG
	assert(!hasValue(key, *base));
	assert(size == countValue(*base));
	#endif
	return NULL;
}

/**
 * When found, the element is removed from the chain and modified to completely separate it.
 * It is then returned.
 */
struct value *removeValueExact(struct value *key, struct value **base){
	if (!*base || !key) return NULL;
	struct value *t = NULL, *prev = NULL;

	#ifdef DEBUG
	int size = countValue(*base);
	#endif

	for(t = *base; t; t = t->next){
		if (t == key){
			if (prev)	prev->next 	= t->next; /* skip over current */
			else			*base 			= t->next; /* set front to next */
			t->next = NULL; /* erase link */

			#ifdef DEBUG
			assert(!hasValueExact(t, *base));
			assert(size - 1 == countValue(*base));
			#endif
			return t;
		}

		prev = t;
	}

	#ifdef DEBUG
	assert(!hasValueExact(key, *base));
	assert(size == countValue(*base));
	#endif
	return NULL;
}

/**
 * Iteratively performs a reversal algorithm on the elements until the terminal element is reached.
 * The new head is then set to the input pointer.
 */
void reverseValue(struct value **base){
	struct value *prev = NULL, *curr = *base, *next = NULL;

	while(curr){
		next = curr->next;
		curr->next = prev;
		prev = curr;
		curr = next;
	}
	*base = prev;

}


/**
 * Uses the ability to start a new chain from @ref newValue to create a chain with the value of input.
 */
struct value *copyValue(struct value *base){
	return newValue(base->i, NULL);
}

/**
 * Displays a warning if the minimum is greater than the maximum.
 */
struct value *newValueChain(int min, int max){
	if (min > max) {printf("warning: min=%d > max=%d, empty die\n",min,max); max = min;}
	struct value *val1 = NULL;
	for (int i = max; i >= min; i--)
		val1 = newValue(i, val1);
	return val1;
}


/**
 * Iteratively increments a counter.
 */
int countValue(struct value *base){
	struct value *t = NULL;
	int c = 0;

	for(t = base; t; t = t->next)	c++;

	#ifdef DEBUG
	assert(c >= 0);
	#endif
	return c;
}

/**
 * Iteratively accumulates the individual element's values.
 */
int sumValue(struct value *base){
		struct value *t = NULL;
		int s = 0;

		#ifdef DEBUG
		int size = countValue(base);
		#endif

		for(t = base; t; t = t->next)	s += t->i;

		#ifdef DEBUG
		assert(size == countValue(base));
		#endif
		return s;
}

/**
 * Checks for an exact struct instance by comparing memory addresses of the pointers.
 */
bool hasValueExact(struct value *key, struct value *base){
		struct value *t = NULL;

		#ifdef DEBUG
		//assert(countValue(key) == 1); /* disallow chains */
		#endif

		for(t = base; t; t = t->next){
			if (t == key){
				return true;
			}
		}

		return false;
	}

/**
 * Checks for an exact struct instance by comparing values of each element.
 */
bool hasValue(int key, struct value *base){
	struct value *t = NULL;

	for(t = base; t; t = t->next){
		if (t->i == key) return true;
	}

	return false;
}



/* === MEMORY MANAGEMENT === */

/**
 * Iteratively frees elements within a value chain.
 * Each element's location data is also set to NULL.
 */
void freeValue( struct value **val ){
	struct value *nval = NULL;
  while(*val){
    nval = (*val)->next;
    free(*val);
    *val = NULL;
    *val = nval;
  }

	#ifdef DEBUG
	assert(!*val);
	#endif
}


/* ======= DEBUGGING ======= */

/**
 * Iteratively outputs the values of a chain.
 */
void printValue(struct value *base){
	struct value *t;
	for (t = base; t; t = t->next)
		printf("%d ",t->i);
	//printf("\n");
}
