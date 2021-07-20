/**
 * @file eval.c
 */
#include <stdio.h> /* printf */
#include <math.h> /* pow */


#ifdef DEBUG
#include <assert.h> /* assert */
#endif

#include "parser.tab.h" /* DIV, INTER, UNION */
#include "eval.h"


/** Tests a result struct to ensure relevant fields are filled and others are not.
 * @param[in] res The result struct to be tested.
 */
void ensureType(struct result *res);

/**
 * Allocates memory for a new struct.
 * If memory cannot be allocated, an error is thrown.
 * If prev is NULL, a new selection chain is created.
 * Returns NULL if memory could not be allocated.
 */
struct selection *newSelection(struct value *val, struct selection *prev){
	struct selection *a = malloc(sizeof(struct selection));

	if (!a){
		printf("out of space\n");
		return NULL;
	}

	a->val = val;
	a->next = prev;
	#ifdef DEBUG
	assert(a && a->val == val && a->next == prev);
	#endif
	return a;
}

/**
 * Aliases the back element of a selection.
 */
struct selection *backSelection(struct selection *base){
	struct selection *t = NULL;
	for (t = base; t->next; t = t->next){ /* get to the end */ }

	#ifdef DEBUG
	assert(t && t->val && !t->next);
	#endif
	return t;
}

/**
 * Aliases the found element
*/
struct selection *findSelection(int key, struct selection *base){
	if (!base) return NULL;
	struct selection *t = NULL;

	for(t = base; t; t = t->next){
		if ( t->val->i == key ) return t;
	}
	/*#ifdef DEBUG
	assert(!hasSelection(key,base));
	#endif*/
	return NULL;
}

/**
 * Aliases the found element
 */
struct selection *findSelectionExact(struct value *key, struct selection *base){
	if (!base) return NULL;
	struct selection *t = NULL;

	for(t = base; t; t = t->next){
		if ( t->val == key ) return t;
	}
	#ifdef DEBUG
	assert(!hasSelection(key,base));
	#endif
	return NULL;
}

/**
 * Removes the first selected element with the value key from base and returns the removed element.
 * base is modified, so a pointer pointer is required.
 */
struct selection *removeSelection(int key, struct selection **base){
	if (!*base) return NULL;
	struct selection *t = NULL, *prev = NULL;

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
 * Removes the exact element key from base.
 * base is modified, so a pointer pointer is required.
 */
struct selection *removeSelectionValue(struct value *key, struct selection **base){
	if (!*base) return NULL;
	struct selection *t = NULL, *prev = NULL;

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
 * Removes an exact selection struct from a chain.
 * base is modified, so a pointer pointer is required.
 */
struct selection *removeSelectionExact(struct selection *key, struct selection **base){
	if (!*base) return NULL;
	struct selection *t = NULL, *prev = NULL;

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
 *
 */
struct selection *copySelection(struct selection *base){
	return newSelection(newValue(base->val->i, NULL), NULL);
}


/**
 * Iteratively increments a counter for each element.
 */
int countSelection(struct selection *base){
	struct selection *t = NULL;
	int c = 0;
	for (t = base; t; t = t->next) c++;

	#ifdef DEBUG
	assert(c >= 0);
	#endif
	return c;
}

/**
 * Iteratively checks for the input key.
 */
bool hasSelectionExact(struct selection *key, struct selection *base){
	if (!base) return false;
	struct selection *t = NULL;

	for(t = base; t; t = t->next){
		if (t == key){
			return true;
		}
	}
	return false;
}

/**
 * Iteratively checks for the input key.
 */
bool hasSelection(struct value *key, struct selection *base){
	if (!base) return false;
	struct selection *t = NULL;

	for(t = base; t; t = t->next){
		if (t->val == key){
			return true;
		}
	}
	return false;
}

/**
 * Frees all contained values within a result variable.
 * Result values should only be aliased within functions and never passed between, reducing risk of memory problems.
 */
void freeResult(struct result **res){
	switch ((*res)->type){
	case R_roll:	freeValue(&(*res)->faces);
	case R_set:		freeValue(&(*res)->out); break;
	case R_die:		freeValue(&(*res)->faces); break;
	case R_int:
	}

	#ifdef DEBUG
	assert(!(*res)->faces && !(*res)->out);
	#endif

	free(*res);
	*res = NULL;

	#ifdef DEBUG
	assert(!*res);
	#endif
}

/** Frees memory allocated to a selection chain.
 * Since selection values are aliased, this function only frees the selection nodes, not their values.
 * @param[in,out] res The node to free.
 */
void freeSelection(struct selection **sel){
struct selection *nsel = NULL;
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


/**
 *
 */
struct result *eval(struct ast *base){
	struct result *r = malloc(sizeof(struct result));
	r->faces = NULL; r->out = NULL; /* set unuseds to NULL */

	if (!r){
		printf("out of space\n");
		exit(1);
	}

	switch (base->nodetype){
	case '+': case '-': case '*': case DIV: case '%': case '^': {
		r->type = R_int;
		struct result *larg = eval(base->l);
		struct result *rarg = eval(base->r);
		switch (base->nodetype){
		case '+': r->integer = larg->integer + rarg->integer; break;
		case '-': r->integer = larg->integer - rarg->integer; break;
		case '*': r->integer = larg->integer * rarg->integer; break;
		case DIV: r->integer = larg->integer / rarg->integer; break;
		case '%': r->integer = larg->integer % rarg->integer; break;
		case '^': r->integer = pow(larg->integer, rarg->integer); break;
		}
		freeResult( &larg );
		freeResult( &rarg );
		break;
	}

	case '1': /* > */
	case '2': /* < */
	case '3': /* != */
	case '4': /* == */
	case '5': /* >= */
	case '6': /* <= */
	case 'M':

	case '&':
	case '|':
	case INTER:
	case UNION:

	case 'e': /* append */
	case 'f': /* drop */
	case 'g': /* count */
	case 'h': /* choose */
	case 'i': /* reroll */

	case 'R': /* roll nat die */
	case 'r': /* roll artificial die */
	case 'Z': /* strip faces */
	case 'S': /* sum roll values */

	case 'D': /* natural die definition */
	case 'd': /* artificial die def. */
	case 'I': /* natural integer */
		r->type = R_int;
		r->integer = ((struct natint *)base)->integer;
		break;
	case 'C': /* function arguments */
	case 'Q': /* artificial roll */
	case 'E': /* sym call */
	case 'A': /* sym definition */
	}

	return r;
}
