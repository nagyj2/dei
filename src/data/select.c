/** Implementation for the @ref SelectionChain struct.
 * @file select.c
 * @author Jason Nagy (jaysun_n@hotmail.com)
 * @version 1.0
 * @date 2021-07-26
 * 
 * @copyright Copyright (c) 2021
 * 
 */

#include <stdlib.h> /* malloc, NULL */
#include <stdio.h> /* printf */

#ifdef DEBUG
#include <assert.h> /* assert */
#endif

#include "select.h"



/* ===== FUNCTIONS ===== */

/**
 * Allocates memory for a new struct.
 * Returns NULL if memory could not be allocated.
 */
SelectionChain *newSelection(ValueChain *val, SelectionChain *prev){
	SelectionChain *a = malloc(sizeof(SelectionChain));

	if (!a){
		printf("out of space\n");
		return NULL;
	}

	if (val == NULL) {
		printf("NULL selection!\n");
		exit(4);
	}

  /** TODO: check and ensure prev doesnt have val already  */
	a->val = val;
	a->next = prev;
  #ifdef DEBUG
  assert(a && a->val == val && a->next == prev);
	#endif
	return a;
}

/**
 * Aliases the back element of a selection.
 * Iteratively searches for the final element in @p base.
 */
SelectionChain *backSelection(SelectionChain *base) {
	if (base == NULL) return NULL;
	SelectionChain *t = NULL;
	for (t = base; t->next; t = t->next){ /* get to the end */ }

	#ifdef DEBUG
	assert(t && t->val && !t->next);
	#endif
	return t;
}

/**
 * Searches for and then aliases the found element.
 * Iteratively searches for @p key.
 * Short-curcuits when @p key is found.
 */
SelectionChain *findSelection(int key, SelectionChain *base){
	if (!base) return NULL;
	SelectionChain *t = NULL;

	for(t = base; t; t = t->next){
		if ( t->val->i == key ) return t;
	}
	/*#ifdef DEBUG
	assert(!hasSelection(key,base));
	#endif*/
	return NULL;
}

/**
 * Searches for and an exact memory address within its values then aliases the found element.
 * Iteratively searches for @p key.
 * Short-curcuits when @p key is found.
 */
SelectionChain *findSelectionExact(ValueChain *key, SelectionChain *base){
	if (!base) return NULL;
	SelectionChain *t = NULL;

	for(t = base; t; t = t->next){
		if ( t->val == key ) return t;
	}
	#ifdef DEBUG
	assert(!hasSelection(key,base));
	#endif
	return NULL;
}

/**
 * Iteratively searches for an integer key contained within a selected @ref ValueChain and removes it if found.
 * The searched-for element has its next attribute modified to remove @p key from the chain.
 * Short-curcuits when @p key is found.
 */
SelectionChain *removeSelection(int key, SelectionChain **base){
	if (!*base) return NULL;
	SelectionChain *t = NULL, *prev = NULL;

	#ifdef DEBUG
	int size = countSelection(*base);
	#endif

	for(t = *base; t; t = t->next){
		if (t->val->i == key){
			if (prev)	prev->next 	= t->next; /* skip over current */
			else 			*base 			= t->next; /* move head to next */
			t->next = NULL; /* Erase link */

			#ifdef DEBUG
			assert(!hasSelectionExact(t, *base));
			assert(size - 1 == countSelection(*base));
			#endif
			return t;
		}
		prev = t;
	}

	#ifdef DEBUG
	/* assert(!hasSelection(key, *base)); */
	assert(size == countSelection(*base));
	#endif
	return NULL;
}

/**
 * Iteratively searches for an exact key address contained within a @ref SelectionChain and removes it if found.
 * To check an element, the memory addresses are compared, so this method works only for aliases.
 * The searched-for element has its next attribute modified to remove @p key from the chain.
 * Short-curcuits when @p key is found.
 * @p key and the returned value are the same memory address.
 */
SelectionChain *removeSelectionValue(ValueChain *key, SelectionChain **base){
	if (!*base) return NULL;
	SelectionChain *t = NULL, *prev = NULL;

	#ifdef DEBUG
	int size = countSelection(*base);
	#endif

	for(t = *base; t; t = t->next){
		if (t->val == key){
			if (prev)	prev->next 	= t->next; /* skip over current */
			else 			*base 			= t->next; /* move head to next */
			t->next = NULL; /* Erase link */

			#ifdef DEBUG
			assert(!hasSelectionExact(t, *base));
			assert(size - 1 == countSelection(*base));
			#endif
			return t;
		}
		prev = t;
	}

	#ifdef DEBUG
	/* assert(!hasSelection(key, *base)); */
	assert(size == countSelection(*base));
	#endif
	return NULL;
}

/**
 * Iteratively searches for an exact @ref SelectionChain in another and removes it if found.
 * To check an element, the memory addresses are compared, so this method works only for aliases.
 * The searched-for element has its next attribute modified to remove @p key from the chain.
 * Short-curcuits when @p key is found.
 * @p key and the returned value are the same memory address.
 */
SelectionChain *removeSelectionExact(SelectionChain *key, SelectionChain **base){
	if (!*base) return NULL;
	SelectionChain *t = NULL, *prev = NULL;

	#ifdef DEBUG
	int size = countSelection(*base);
	#endif

	for(t = *base; t; t = t->next){
		if (t == key){
			if (prev)	prev->next 	= t->next; /* skip over current */
			else 			*base 			= t->next; /* move head to next */
			t->next = NULL; /* Erase link */

			#ifdef DEBUG
			assert(!hasSelectionExact(t, *base));
			assert(size - 1 == countSelection(*base));
			#endif
			return t;
		}
		prev = t;
	}

	#ifdef DEBUG
	/* assert(!hasSelection(key, *base)); */
	assert(size == countSelection(*base));
	#endif
	return NULL;
}


/**
 * Creates a new @ref SelectionChain by aliasing the contents of another.
 * Uses newSelection()'s ability to create a new @ref ValueChain head.
 * Copys the value of @p base by accessing its integer value.
 */
SelectionChain *copySelection(SelectionChain *base){
	return newSelection(base->val, NULL);
	// return newSelection(newValue(base->val->i, NULL), NULL);
}


/**
 * Iteratively searches through a @ref SelectionChain and counts the number of jumps performed.
 */
int countSelection(SelectionChain *base){
	SelectionChain *t = NULL;
	int c = 0;
	for (t = base; t; t = t->next) c++;

	#ifdef DEBUG
	assert(c >= 0);
	#endif
	return c;
}

/**
 * Checks for an exact @ref SelectionChain instance by comparing memory addresses of pointers.
 * Since this function operates on comparison of memory addresses, it will only return true if @p key is an alias.
 * Short-circuits on a found element.
 */
bool hasSelectionExact(SelectionChain *key, SelectionChain *base){
	if (!base) return false;
	SelectionChain *t = NULL;

	for(t = base; t; t = t->next){
		if (t == key){
			return true;
		}
	}
	return false;
}

/**
 * Checks for an exact @ref ValueChain instance by comparing memory addresses of pointers.
 * Since this function operates on comparison of memory addresses, it will only return true if @p key is an alias.
 * Short-circuits on a found element.
 */
bool hasSelection(ValueChain *key, SelectionChain *base){
	if (!base) return false;
	SelectionChain *t = NULL;

	for(t = base; t; t = t->next){
		if (t->val == key){
			return true;
		}
	}
	return false;
}

/**
 * Checks for a @ref SelectionChain instance which has a pointed to @ref ValueChain with a specific integer value.
 * Short-circuits on a found element.
 */
bool hasSelectionInt(int key, SelectionChain *base){
	if (!base) return false;
	SelectionChain *t = NULL;

	for(t = base; t; t = t->next){
		if (t->val->i == key){
			return true;
		}
	}
	return false;
}



/* ===== MEMORY MANAGEMENT ===== */

/**
 * Iteratively frees memory allocated to a @ref SelectionChain and its values.
 * As a side effect, all references to values in the chain will be set to NULL silently.
 */
void freeSelectionComplete(SelectionChain **sel) {
  SelectionChain *nsel = NULL;
  while(*sel){
    nsel = (*sel)->next;
    free( (*sel)->val );
    (*sel)->val = NULL;
    #ifdef DEBUG
    assert(!(*sel)->val);
    #endif
    free(*sel);
    *sel = NULL;
    *sel = nsel;
  }

  #ifdef DEBUG
  assert(!*sel);
  #endif
}

/**
 * Iteratively frees memory allocated to a @ref SelectionChain.
 * As a side effect, all references to values in the chain will be set to NULL silently.
 */
void freeSelectionAliased(SelectionChain **sel) {
  SelectionChain *nsel = NULL;
  while(*sel){
    nsel = (*sel)->next;
    free(*sel);
    *sel = NULL;
    *sel = nsel;
  }

  #ifdef DEBUG
  assert(!*sel);
  #endif
}



/* ===== DEBUGGING ===== */

/**
 * Iteratively outputs the values of a chain to standard output.
 */
void printSelection(SelectionChain *sel) {
  SelectionChain *t = NULL;
  for (t = sel; t; t = t->next) {
    printf("S(%d:%p), ", t->val->i, t->val);
  }
}
