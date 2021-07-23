/**
 * @file eval.c
 * @author Jason Nagy (jaysun_n@hotmail.com)
 * @version 0.1
 * @date 2021-07-23
 * 
 * @copyright Copyright (c) 2021
 * 
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


int randint(int min, int max){
	return (rand() % (max + 1 - min)) + min;
}

struct value *rollSet(int times, struct value *faces){
	struct value *r = NULL, *t = NULL;
	int len = countValue(faces);
	for (int i = 0; i < times; i++){
		int j = randint(0, len-1); /* index by element */
		for (t = faces; j > 0; t = t->next, j--) { /* skip */ }
		r = newValue(t->i, r); /* use the element */
	}
	return r;
}

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
 * Iteratively checks for the input key.
 */
bool hasSelectionInt(int key, struct selection *base){
	if (!base) return false;
	struct selection *t = NULL;

	for(t = base; t; t = t->next){
		if (t->val->i == key){
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

/** Frees memory allocated to a selection chain and the pointers contained within
 * For appending operations, new value structs are created, so they must be cleaned specially.
 * @param[in,out] res The node to free.
 */
void freeSelectionComplete(struct selection **sel){
struct selection *nsel = NULL;
while(*sel){
	nsel = (*sel)->next;
	if ((*sel)->val) free( (*sel)->val );
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

void freeSelectionAliased(struct selection **sel){
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

/** Performs a selection algorithm on @p rolled according to the options in @p opts.
 * @p rolled is not modified, but the output contains pointers to the elements within.
 * @param[in]  rolled The options the algorithm can select from.
 * @param[in]  opts   Selection options, such as times and what to select.
 * @return        A list of selected elements from @p rolled. Elements are aliased.
 */
struct selection *select(struct value *rolled, struct fargs *opts){
	// if scount == -1 at start, set times to iterate to the same size as rolled
	struct selection *sel = NULL;
	int elem = countValue(rolled) - 1, times = opts->scount;
	if (elem == 0) return NULL;

	if (opts->seltype > 0 && opts->scount == -1){
		times = elem; /* if set to find all, set search times equal to length */
	}

	do {
		struct value *t = NULL;
		int index = randint(0, elem);

		switch (opts->seltype) {
		case S_high: /* find a highest element */
			for (t = rolled; t; t = t->next) /* find first unselected and select it */
				if (!hasSelection(t, sel))
				{
					sel = newSelection(t, sel); /* ALIAS! */
					break;
				}
			for (t = rolled; t; t = t->next)\
				if (t->i > sel->val->i && !hasSelection(t, sel))
					sel->val = t; /* ALIAS! */

			#ifdef DEBUG
			assert(sel->val);
			#endif
			break;
		case S_low: /* find the lowest element */
			for (t = rolled; t; t = t->next) /* find first unselected and select it */
				if (!hasSelection(t, sel))
				{
					sel = newSelection(t, sel); /* ALIAS! */
					break;
				}
			for (t = rolled; t; t = t->next)\
				if (t->i < sel->val->i && !hasSelection(t, sel))
					sel->val = t; /* ALIAS! */

			#ifdef DEBUG
			assert(sel->val);
			#endif
			break;
		case S_rand: /* find a random element */
			for(t = rolled; index-- > 0; t = t->next){ /* reduce index */ }
			sel = newSelection(t, sel); /* ALIAS! */
			#ifdef DEBUG
			assert(sel->val);
			#endif
			break;
		case S_unique: /* find first unselected and select it */
			sel = newSelection(rolled, sel); /* S_unique has scount = 1 guarenteed */

			for(t = rolled; t; t = t->next){ /* iteratively find uniques */
				if (!hasSelectionInt(t->i, sel)){
					sel = newSelection(t, sel); /* ALIAS! */
				}
			}
			#ifdef DEBUG
			assert(sel->val);
			#endif
			break;
		default:
			for(t = rolled; t; t = t->next){ /* iteratively find uniques */
				if (opts->seltype == t->i && !hasSelection(t,sel)){
					sel = newSelection(t, sel); /* ALIAS! */
					break;
				}
			}
			break;
		}

	} while (--times > 0);
	return sel;
}

/** Generates a selection of value structs that are NOT contained within rolled.
 * These new elements may be based off of rolled, but the output does not contain pointers to existing elements.
 * These selected elements will eventually be placed into rolled.
 * @param[in]  rolled Information the generation algorithm can act on.
 * @param[in]  opts   Selection options, such as times and what to select.
 * @return        A list of selected elements NOT from rolled.
 */
struct selection *generate(struct value *rolled, struct fargs *opts){
	struct selection *sel = NULL;
	int times = opts->scount;
	/* lengths of current and past selection. Used to detect when there is no length change. length of input chain */
	int len = 0, prev = 0, elem = countValue(rolled) - 1; 

	do {
		struct value *t = NULL;
		int index = randint(0, elem); /* index to take from for random */
		
		switch (opts->seltype){
		case S_high: /* find the highest element */
			for(t = rolled; t; t = t->next){ /* find first unselected and select it */
				if (!hasSelection(t, sel)){
					sel = newSelection(copyValue(t), sel); /* DONT ALIAS! */
					break;
				}
			}
			for(t = rolled; t; t = t->next){ /* iteratively find the highest */
				if (t->i > sel->val->i && !hasSelection(t, sel)){
					sel->val = copyValue(t); /* DONT ALIAS! */
				}
			}
			#ifdef DEBUG
			assert(sel->val);
			#endif
			break;
		case S_low: /* find the lowest element */
			for(t = rolled; t; t = t->next){ /* find first unselected and select it */
				if (!hasSelection(t, sel)){
					sel = newSelection(copyValue(t), sel); /* DONT ALIAS! */
					break;
				}
			}
			for(t = rolled; t; t = t->next){ /* iteratively find the highest */
				if (t->i < sel->val->i && !hasSelection(t, sel)){
					sel->val = copyValue(t); /* DONT ALIAS! */
				}
			}
			#ifdef DEBUG
			assert(sel->val);
			#endif
			break;
		case S_rand: /* select a random element */
			for(t = rolled; index-- > 0; t = t->next){ /* reduce index */ }
			sel = newSelection(copyValue(t), sel); /* DONT ALIAS! */
			#ifdef DEBUG
			assert(sel->val);
			#endif
			break;
		case S_unique: /* find first unselected and select it */
			sel = newSelection(copyValue(rolled), sel); /* S_unique has scount = 1 guarenteed */

			for(t = rolled; t; t = t->next){ /* iteratively find uniques */
				if (!hasSelectionInt(t->i, sel)){
					sel = newSelection(copyValue(t), sel); /* DONT ALIAS! */
				}
			}
			#ifdef DEBUG
			assert(sel->val);
			#endif
			break;
		default: /* numbers */
			sel = newSelection(newValue(opts->seltype, NULL), sel);
		}

		prev = len;
		len = countSelection(sel);
		if (prev == len){ return sel; } /* no more changes */
	} while (--times > 0);
	return sel;
}


/**
 *
 */
struct result *eval(struct ast *base){
	struct result *r = malloc(sizeof(struct result));
	r->faces = NULL; r->out = NULL; /* set unuseds to NULL */
	r->integer = 0; /* unused ->keep? */

	if (!r){
		printf("out of space\n");
		exit(1);
	}

	switch (base->nodetype){
	case '+': case '-': case '*': case DIV: case '%': case '^':
	case '1': /* > */ 	case '2': /* < */ 	case '3': /* != */
	case '4': /* == */ 	case '5': /* >= */ 	case '6': /* <= */ {
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
		case '1': r->integer = larg->integer > rarg->integer; break;
		case '2': r->integer = larg->integer < rarg->integer; break;
		case '3': r->integer = larg->integer != rarg->integer; break;
		case '4': r->integer = larg->integer == rarg->integer; break;
		case '5': r->integer = larg->integer >= rarg->integer; break;
		case '6': r->integer = larg->integer <= rarg->integer; break;
		}
		freeResult( &larg );
		freeResult( &rarg );
		break;
	}

	case 'M': {
		r->type = R_int;
		struct result *larg = eval(base->l);
		r->integer = -(larg->integer);
		freeResult( &larg );
		break;
	}

	case '&': case INTER:	{
		r->type = R_set;
		struct result *larg = eval(base->l);
		struct result *rarg = eval(base->r);
		struct value *t = NULL;
		if (base->nodetype == INTER) r->out = dupValue(larg->out); /* if '&', start from NULL */
		for(t = rarg->out; t; t = t->next){
			if (hasValue(t->i,larg->out)) r->out = newValue(t->i, r->out);
		}
		freeResult( &larg );
		freeResult( &rarg );
		break;
	}
	case '|': case UNION: {
		r->type = R_set;
		struct result *larg = eval(base->l);
		struct result *rarg = eval(base->r);
		struct value *t = NULL;
		r->out = dupValue(larg->out);
		if (base->nodetype == '|'){
			for(t = rarg->out; t; t = t->next){
				if (!hasValue(t->i, r->out))
				r->out = newValue(t->i, r->out);
			}
		}else{
			for(t = rarg->out; t; t = t->next){
				r->out = newValue(t->i, r->out);
			}
		}
		freeResult( &larg );
		freeResult( &rarg );
		break;
	}

	case 'e': /* append */ {
		r->type = R_roll;
		struct result *inputs = eval( base->l ); /* find the outputs from the contained tree */
		r->out = dupValue(inputs->out); /* duplicate entire chain */
		struct selection *selected = generate(r->out, (struct fargs *) base->r), *t = NULL;

		for(t = selected; t; t = t->next){
			switch(base->nodetype){
			case 'e': /* append */
				r->out = newValue(t->val->i, r->out);
			}
		}
		freeResult( &inputs );
		freeSelectionComplete( &selected );
		break;
	}

	case 'f': /* drop */ {
		r->type = R_roll;
		struct result *inputs = eval( base->l ); /* find the outputs from the contained tree */
		r->out = dupValue(inputs->out); /* duplicate entire chain */
		struct selection *selected = select(r->out, (struct fargs *) base->r), *t = NULL;

		if(selected){
			for(t = selected; t; t = t->next){
				removeValueExact(t->val, &(r->out));
				/* removed elements are still pointed to by selected, so we dont care about the output */
			}

			freeResult(&inputs);
			freeSelectionComplete( &selected );
		}
		break;
	}
	case 'g': /* count */ {
		break;
	}
	case 'h': /* choose */ {
		r->type = R_roll;
		struct result *inputs = eval( base->l ); /* find the outputs from the contained tree */
		r->out = NULL; /* Start with nothing */
		struct selection *selected = select(inputs->out, (struct fargs *)base->r), *t = NULL;

		if(selected){
			for(t = selected; t; t = t->next){
				r->out = newValue(t->val->i, r->out);
				/* copy each selected element */
			}

			//freeSelectionComplete( &selected );
			freeResult(&inputs);
		}
		break;
	}

	case 'i': /* reroll */

		break;

	case 'R': /* roll nat die */ case 'r': /* roll artificial die */ {
		r->type = R_roll;
		struct result *die = eval(base->l);
		r->faces = dupValue(die->faces);
		r->out = rollSet(die->integer, r->faces);
		freeResult( &die );
		break;
	}
	case 'Z': /* strip faces */ {
		r->type = R_set;
		struct result *roll = eval(base->l);
		r->out = dupValue(roll->out);
		freeResult( &roll );
		break;
	}
	case 'S': /* sum roll values */{
		r->type = R_int;
		struct result *set = eval(base->l);
		r->integer = sumValue(set->out);
		freeResult( &set );
		break;
	}
	case 'D': /* natural die definition */
		r->type = R_die;
		r->faces = newValueChain(((struct natdie *)base)->min, ((struct natdie *)base)->max);
		r->integer = ((struct natdie *)base)->count;
		break;
	case 'd': /* artificial die def. */
		r->type = R_die;
		r->faces = dupValue(((struct setdie *)base)->faces);
		r->integer = ((struct natdie *)base)->count;
		break;
	case 'I': /* natural integer */
		r->type = R_int;
		r->integer = ((struct natint *)base)->integer;
		break;
	case 'Q': /* artificial roll */ {
		r->type = R_roll;
		r->out = dupValue(((struct setres *)base)->out);
		break;
	}
	case 'C': /* function arguments */
	case 'E': /* sym call */ {
		free(r);
		r = eval(((struct symcall *)base)->sym->func);
		break;
	}
	case 'A': /* sym definition */ {
		r->type = R_int;
		setsym(((struct astAsgn *)base)->s, ((struct astAsgn *)base)->l);
		r->integer = 0; /* signal success */
		break;
	}
	default:
		printf("unknown eval type, got %d",base->nodetype);
		exit(3);
	}

	return r;
}
