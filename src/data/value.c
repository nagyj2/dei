/** Implements a singlely linked-list implementation to store integer chains.
 * The start of the chain, the head, is kept as a pointer and all other elements need to be iterated to.
 * Chains cannot be traversed in the reverse direction because of the single-link structure.
 * @file struct.c
 * @author Jason Nagy (jaysun_n@hotmail.com)
 * @version 0.1
 * @date 2021-07-26
 * 
 * @copyright Copyright (c) 2021
 * 
 */

#include <stdlib.h> /* malloc, exit */
#include <stdio.h> /* printf */

#ifdef DEBUG
#include <assert.h> /* assert */
#endif

#include "value.h"


/* ===== FUNCTIONS ===== */

/**
 * Allocates new memory for the struct and then populates its values.
 * Returns NULL if memory could not be allocated.
 */
ValueChain *newValue(int i, ValueChain *prev){
	ValueChain *a = malloc(sizeof(ValueChain));
	if (!a){
		printf("out of space");
		return NULL;
	}

	a->i = i;
	a->next = prev; /* Dont need to check because NULL means a new chain and non-NULL means a new head */
	#ifdef DEBUG
	assert(a && a->i == i && a->next == prev);
	#endif
	return a;
}

/**
 * Iteratively creates new value elements connected to one another. Afterwards, the chain is reversed.
 * If any element could not be created, NULL will be returned.
 */
ValueChain *dupValue(ValueChain *base) {
	ValueChain *head = NULL, *t = NULL;

	for(t = base; t; t = t->next){
		head = newValue(t->i, head);
	}

	reverseValue( &head );

	#ifdef DEBUG
	ValueChain *h = NULL;
	for(t = base, h = head; t || h; t = t->next, h = h->next){
		assert(t->i == h->i);
	}
	#endif

	return head;
}

/**
 * Iteratively work through the chain until the next element is NULL, signifying the last element.
 * Returns an alias to the final element.
 */
ValueChain *backValue(ValueChain *base){
	ValueChain *t = NULL;

	if (!base)	return NULL;
	
	for (t = base; t->next; t = t->next) { /* Skip */ }

	#ifdef DEBUG
	// assert(t && t->i && !t->next);
	#endif
	return t;
}

/**
 * Iteratively search for the integer key and return the first match.
 * Short-circuits on a found element.
 * When a match is found, an alias of the element is returned.
 */
ValueChain *findValue(int key, ValueChain *base){
	if (!base) return NULL;
	ValueChain *t = NULL;

	for(t = base; t; t = t->next){
		if (t->i == key) return t;
	}
	#ifdef DEBUG
	assert(!hasValue(key, base));
	#endif
	return NULL;
}

/**
 * Iteratively searches for an integer key and removes it when found.
 * When found, the value is extracted by modifying the previous element's next attribute.
 * The searched-for element has its next attribute modified to remove it from the chain.
 */
ValueChain *removeValue(int key, ValueChain **base){
	if (!*base) return NULL;
	ValueChain *t = NULL, *prev = NULL;

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
 * Iteratively searches for an exact key and removes it when found.
 * To check an element, the memory addresses are compared, so this method works only for aliases.
 * The searched-for element has its next attribute modified to remove @p key from the chain.
 * Short-curcuits when @p key is found.
 * @p key and the returned value are the same memory address.
 */
ValueChain *removeValueExact(ValueChain *key, ValueChain **base) {
	if (!*base || !key) return NULL;
	ValueChain *t = NULL, *prev = NULL;

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
 * Operates by operating on 3 elements concurrently.
 * Any aliases which exist will be silently modified.
 * @p base is then set to the last element.
 */
void reverseValue(ValueChain **base) {
	ValueChain *prev = NULL, *curr = *base, *next = NULL;

	while(curr){
		next = curr->next;	/* Save next element */
		curr->next = prev;	/* Set "next" element to previous */
		prev = curr;				/* Set last element to current */
		curr = next;				/* Set current to the saved next element */
	}
	*base = prev;					/* Update head to the last element */

}


/**
 * Creates a new element by making a copy of another.
 * Uses newValue()'s ability to create a new @ref ValueChain head.
 * Copys the value of @p base by accessing its integer value.
 */
ValueChain *copyValue(ValueChain *base) {
	return newValue(base->i, NULL);
}

/**
 * Iteratively creates a new @ref ValueChain by exploiting newValue()'s ability to chain elements.
 * Displays a warning if @p min is greater than @p max and forces a 1 element chain with a value of @p min.
 */
ValueChain *newValueChain(int min, int max) {
	if (min > max) {printf("warning: min=%d > max=%d, empty die\n",min,max); max = min;}
	ValueChain *val1 = NULL;
	for (int i = max; i >= min; i--)
		val1 = newValue(i, val1);
	return val1;
}


/**
 * Iteratively searches through a @ref ValueChain and counts the number of jumps performed.
 */
int countValue(ValueChain *base) {
	ValueChain *t = NULL;
	int c = 0;

	for(t = base; t; t = t->next)	c++;

	#ifdef DEBUG
	assert(c >= 0);
	#endif
	return c;
}

/**
 * Iteratively advances through a @ref ValueChain and accumulates the individual element's values.
 */
int sumValue(ValueChain *base) {
	ValueChain *t = NULL;
	int s = 0;

	#ifdef DEBUG
	int size = countValue(base);
	#endif

	for (t = base; t; t = t->next)
		s += t->i;

	#ifdef DEBUG
	assert(size == countValue(base));
	#endif
	return s;
}

/**
 * Checks for an exact struct instance by comparing memory addresses of the pointers.
 * Since this function operates on comparison of memory addresses, it will only return true if @p key is an alias.
 * Short-circuits on a found element.
 */
bool hasValueExact(ValueChain *key, ValueChain *base) {
	if (!base)	return false;
	ValueChain *t = NULL;
	
	#ifdef DEBUG
	//assert(countValue(key) == 1); /* disallow chains */
	#endif

	for(t = base; t; t = t->next) {
		if (t == key){
			return true;
		}
	}

	return false;
}

/**
 * Checks for an exact struct instance by comparing values of each element.
 * Short-circuits on a found element.
 */
bool hasValue(int key, ValueChain *base) {
	if (!base)	return false;
	ValueChain *t = NULL;

	for(t = base; t; t = t->next){
		if (t->i == key) return true;
	}

	return false;
}


/* ===== MEMORY MANAGEMENT ===== */

/**
 * Iteratively frees memory allocated to a @ref ValueChain.
 * Additionally, data at each element's location is set to NULL.
 * As a side effect, all references to values in the chain will be set to NULL silently.
 */
void freeValue(ValueChain **val) {
	ValueChain *nval = NULL;
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


/* ===== DEBUGGING ===== */

/**
 * Iteratively outputs the values of a chain to standard output.
 */
void printValue(ValueChain *base) {
	ValueChain *t;
	for (t = base; t; t = t->next)
		printf("%d ",t->i);
	//printf("\n");
}
