/** Implementation of a language evaluator supporting symbols.
 * @file eval.c
 * @author Jason Nagy (jaysun_n@hotmail.com)
 * @version 1.0
 * @date 2021-07-23
 * 
 * @copyright Copyright (c) 2021
 * 
 */

#include <stdlib.h> /* rand */
#include <stdio.h> /* printf */
#include <math.h> /* pow */

#ifdef DEBUG
#include <assert.h> /* assert */
#endif

#include "parser.tab.h" /* DIV, INTER, UNION */
#include "value.h"
#include "eval.h"
#include "select.h"
#include "symbols.h"
#include "result.h"
#include "util.h"


/* ===== FUNCTIONS ===== */

/** Find the maximum value within the input @ref ValueChain.
 * @param[in] faces The die faces to check.
 * @return int 			The largest integer in @p faces.
 */
int maxValue(ValueChain *faces) {
	int max = faces->i;
	for (faces = faces->next; faces; faces = faces->next) {
		if (max < faces->i) {
			max = faces->i;
		}
	}
	return max;
}

/** Find the minimum value within the input @ref ValueChain.
 * @param[in] faces The die faces to check.
 * @return int 			The smallest integer in @p faces.
 */
int minValue(ValueChain *faces) {
	int min = faces->i;
	for (faces = faces->next; faces; faces = faces->next) {
		if (min > faces->i) {
			min = faces->i;
		}
	}
	return min;
}


/** Performs an individual dice rolls and returns the result.
 * Returns a value directly from @p faces, so no new memory is allocated.
 * @param[in] faces The available die faces to roll.
 * @return int 			A single randomly rolled number.
 */
int rollInt(ValueChain *faces) {
	int j = randint(0, countValue(faces) - 1);
	ValueChain *t = NULL;
	for (t = faces; j > 0; t = t->next, j--) { /* skip */ }
	return t->i; /* return result from the face */
}

/** Performs a series of rolls to create a new @ref ValueChain.
 * New memory is allocated to the output.
 * @sideeffect If @var gSilent is 1, notifications of "Nat#" will be sent to standard output when rolling the highest or lowest numbers.
 * @param[in] times 		The number of rolls to make.
 * @param[in] faces 		The faces to roll from.
 * @return ValueChain* 	The set of rolled numbers.
 */
ValueChain *rollSet(int times, ValueChain *faces) {
	ValueChain *r = NULL;
	int max = maxValue(faces), min = minValue(faces), printed = 0;
	for (int i = 0; i < times; i++) {
		int roll = rollInt(faces);
		if (!gSilent && (roll == max || roll == min)) {
			printf("Nat%d", roll);
			printed = 1;
		}
		r = newValue(roll, r); /* use the element */
	}
	if (printed == 1)
		printf(". ");
	return r;
}


/* ===== EVALUATION ===== */

/** Performs a selection algorithm on @p rolled according to the options in @p opts.
 * @p rolled is not modified, but the output contains pointers to the elements within.
 * @param[in]  rolled The options the algorithm can select from.
 * @param[in]  opts   SelectionChain options, such as times and what to select.
 * @return        A list of selected elements from @p rolled. Elements are aliased.
 */
SelectionChain *select(ValueChain *rolled, FuncArgs *opts){
	// if scount == -1 at start, set times to iterate to the same size as rolled
	SelectionChain *sel = NULL;
	int elem = countValue(rolled) - 1, times = opts->scount;
	if (elem == 0) return NULL;

	if (opts->seltype > 0 && opts->scount == -1){
		times = elem; /* if set to find all, set search times equal to length */
	}

	do {
		ValueChain *t = NULL;
		int index = randint(0, elem);

		switch (opts->seltype) {
			case S_high: /* find a highest element */ {
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
			}
			case S_low: /* find the lowest element */ {
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
			}
			case S_rand: /* find a random element */ {
				for (t = rolled; index-- > 0; t = t->next) { /* reduce index */ }
				if (!hasSelection(t, sel)) {
				sel = newSelection(t, sel); /* ALIAS! */
				#ifdef DEBUG
				assert(sel->val);
				#endif
				} else {
					times++;
				}
				break;
			}
			case S_unique: /* find first unselected and select it */ {
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
			}
			default: {
				for(t = rolled; t; t = t->next){ /* iteratively find uniques */
					if (opts->seltype == t->i && !hasSelection(t,sel)){
						sel = newSelection(t, sel); /* ALIAS! */
						break;
					}
				}
				break;
			}
		}

	} while (--times > 0);
	return sel;
}

/** Generates a selection of value structs that are NOT contained within rolled.
 * These new elements may be based off of rolled, but the output does not contain pointers to existing elements.
 * These selected elements will eventually be placed into rolled.
 * @param[in]  rolled Information the generation algorithm can act on.
 * @param[in]  opts   SelectionChain options, such as times and what to select.
 * @return        A list of selected elements NOT from rolled.
 */
SelectionChain *generate(ValueChain *rolled, FuncArgs *opts){
	SelectionChain *sel = NULL;
	int times = opts->scount;
	/* lengths of current and past selection. Used to detect when there is no length change. length of input chain */
	int len = 0, prev = 0, elem = countValue(rolled) - 1; 

	if (opts->seltype == S_unique && times != 1)
		printf("warning: unique selector is assumed to have 1 time, got %d!\n", times);

	do {
		ValueChain *t = NULL;
		int index = randint(0, elem); /* index to take from for random */
		
		switch (opts->seltype){
			case S_high: /* find the highest element */ {
				for(t = rolled; t; t = t->next){ /* find first unselected and select it */
					if (!hasSelection(t, sel)){
						sel = newSelection(copyValue(t), sel); /* DONT ALIAS! */
						break;
					}
				}
				for(t = rolled; t; t = t->next){ /* iteratively find the highest */
					if (t->i > sel->val->i && !hasSelection(t, sel)){
						freeValue(&sel->val);
						sel->val = copyValue(t); /* DONT ALIAS! */
					}
				}
				#ifdef DEBUG
				assert(sel->val);
				#endif
				break;
			}
			case S_low: /* find the lowest element */ {

				for(t = rolled; t; t = t->next){ /* find first unselected and select it */
					if (!hasSelection(t, sel)){
						sel = newSelection(copyValue(t), sel); /* DONT ALIAS! */
						break;
					}
				}

				for(t = rolled; t; t = t->next){ /* iteratively find the highest */
					if (t->i < sel->val->i && !hasSelection(t, sel)){
						freeValue(&sel->val);
						sel->val = copyValue(t); /* DONT ALIAS! */
					}
				}
				#ifdef DEBUG
				assert(sel->val);
				#endif
				break;
			}
			case S_rand: /* select a random element */ {
				for(t = rolled; index-- > 0; t = t->next){ /* reduce index */ }
				sel = newSelection(copyValue(t), sel); /* DONT ALIAS! */
				#ifdef DEBUG
				assert(sel->val);
				#endif
				break;
			}
			case S_unique: /* find first unselected and select it */ {
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
			}
			default: /* numbers */ {
				sel = newSelection(newValue(opts->seltype, NULL), sel);
			}
		}

		prev = len;
		len = countSelection(sel);
		if (prev == len){ return sel; } /* no more changes */
	} while (--times > 0);
	return sel;
}


/** Evaluate the input AST recursively.
 * Execution will not modify the input AST in any way.
 */
Result *eval(AST *base){
	Result *r = malloc(sizeof(Result));
	if (!r){
		printf("out of space\n");
		exit(1);
	}
	
	r->faces = NULL;
	r->out = NULL;	/* set unuseds to NULL */
	r->next = NULL;
	r->integer = 0;

	switch (base->nodetype){
	case '+': case '-': case '*': case DIV: case '%': case '^':
	case '1': /* > */ 	case '2': /* < */ 	case '3': /* != */
	case '4': /* == */ 	case '5': /* >= */ 	case '6': /* <= */ {
		r->type = R_int;
		Result *larg = eval(base->l);
		Result *rarg = eval(base->r);
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
		Result *larg = eval(base->l);
		r->integer = -(larg->integer);
		freeResult( &larg );
		break;
	}

	case '&': case INTER:	{
		r->type = R_set;
		Result *larg = eval(base->l);
		Result *rarg = eval(base->r);
		ValueChain *t = NULL;
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
		Result *larg = eval(base->l);
		Result *rarg = eval(base->r);
		ValueChain *t = NULL;

		for (t = larg->out; t; t = t->next) {
			if (!hasValue(t->i, r->out))
				r->out = newValue(t->i, r->out);
		}
		
		if (base->nodetype == '|') {
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

	case 'e': { /* append */
		r->type = R_roll;
		Result *inputs = eval( base->l ); /* find the outputs from the contained tree */
		r->out = dupValue(inputs->out); /* duplicate entire chain */
		r->faces = dupValue(inputs->faces);
		SelectionChain *selected = generate(r->out, (FuncArgs *) base->r), *t = NULL;

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

	case 'f': { /* drop */
		r->type = R_roll;
		Result *inputs = eval( base->l ); /* find the outputs from the contained tree */
		r->out = dupValue(inputs->out); /* duplicate entire chain */
		r->faces = dupValue(inputs->faces);
		freeResult(&inputs);
		SelectionChain *selected = select(r->out, (FuncArgs *) base->r), *t = NULL;

		if (selected) {
			ValueChain *tt = NULL; /* Hold reference to discarded  */
			for (t = selected; t; t = t->next) {
				/* removed elements are still pointed to by selected, so we dont care about the output */
				tt = removeValueExact(t->val, &(r->out));
				freeValue(&tt); /* free now removed element */
			}

			freeSelectionAliased(&selected);
		}
		break;
	}
	case 'g': { /* count */
		r->type = R_roll;
		Result *inputs = eval( base->l ); /* find the outputs from the contained tree */
		SelectionChain *selected = select(inputs->out, (FuncArgs *)base->r);

		r->out = newValue(countSelection(selected), NULL);

		freeSelectionAliased( &selected );
		freeResult(&inputs);
		break;
	}
	case 'h': { /* choose */
		/* SIGNIFICANT MEMORY LEAK!! */
		r->type = R_roll;
		Result *inputs = eval( base->l ); /* find the outputs from the contained tree */
		ValueChain *outputs = dupValue(inputs->out);
		freeResult(&inputs);

		r->out = NULL; /* Start with nothing */
		SelectionChain *selected = select(outputs, (FuncArgs *) base->r), *t = NULL;

		if (selected) {

			ValueChain *tt = NULL; /* Hold removed elements */
			for (t = selected; t; t = t->next) {
				/* transfer each selected ValueChain */
				tt = removeValueExact(t->val, &outputs);
				#ifdef DEBUG
				assert(tt);
				int l = countValue(r->out);
				#endif
				tt->next = r->out; /* set to new head */
				r->out = tt; /* update head pointer */
				#ifdef DEBUG
				assert(l + 1 == countValue(r->out));
				#endif
			}

			freeSelectionAliased(&selected);
		}
		freeValue(&outputs);
		break;
	}
	case 'i': { /* reroll */
		r->type = R_roll;
		Result *inputs = eval( base->l ); /* find the outputs from the contained tree */
		r->out = dupValue(inputs->out); /* Duplicate start */
		r->faces = dupValue(inputs->faces);
		freeResult(&inputs);

		if (!r->faces) {
			printf("error: reroll needs die faces\n");
			return r;
		}

		SelectionChain *selected = NULL, *t = NULL;
		selected = (countValue(r->out) > 1) ? select(r->out, (FuncArgs *) base->r) : newSelection(r->out, NULL); /* For some reason, select() has problems with single r->out */

		if (selected) {
			int max = maxValue(r->faces), min = minValue(r->faces), printed = 0;
			for (t = selected; t; t = t->next) {
				/* try to reroll each selected once */
				int roll = rollInt(r->faces);
				if (!gSilent && (roll == max || roll == min)) {
					printf("Nat%d", roll);
					printed = 1;
				}
				// printf("%d cmp %d (%d) : Hi:%d, Lo:%d, No:%d\n", t->val->i, roll, ((FuncArgs *) base->r)->cond, C_high, C_low, C_none);
				switch (((FuncArgs *) base->r)->cond) {
				case C_high: 	if (t->val->i < roll) t->val->i = roll; break;
				case C_low: 	if (t->val->i > roll) t->val->i = roll; break;
				case C_none: 	t->val->i = roll; break;
				}
			}
			if (printed == 1)
				printf(". ");
			freeSelectionAliased( &selected );
		}
		break;
	}
	case 'R': /* roll nat die */ case 'r': { /* roll artificial die */
		r->type = R_roll;
		Result *die = eval(base->l);
		r->faces = dupValue(die->faces);
		r->out = rollSet(die->integer, r->faces);
		freeResult( &die );
		break;
	}
	case 'Z': { /* strip faces */
		r->type = R_set;
		Result *roll = eval(base->l);
		r->out = dupValue(roll->out);
		freeResult( &roll );
		break;
	}
	case 'S': { /* sum roll values */
		r->type = R_int;
		Result *set = eval(base->l);
		r->integer = sumValue(set->out);
		freeResult( &set );
		break;
	}
	case 'D': { /* natural die definition */
		r->type = R_die;
		r->faces = newValueChain(((NatDie *)base)->min, ((NatDie *)base)->max);
		r->integer = ((NatDie *)base)->count;
		break;
	}
	case 'd': { /* artificial die def. */
		r->type = R_die;
		r->faces = dupValue(((SetDie *)base)->faces);
		r->integer = ((NatDie *)base)->count;
		break;
	}
	case 'I': { /* natural integer */
		r->type = R_int;
		r->integer = ((NatInt *)base)->integer;
		break;
	}
	case 'Q': { /* artificial roll */
		r->type = R_roll;
		r->out = dupValue(((SetRoll *)base)->out);
		break;
	}
	case 'C': { /* function arguments */
		printf("parsed fargs! should never see this!\n");
		exit(3);
		break;
	}
	case 'E': { /* sym call */
		free(r);
		r = eval(((SymbolRef *)base)->sym->func);
		break;
	}	
	case 'F': { /* if else */
		free(r);
		Result *c = eval(((IfElse *) base)->cond);
		if (c->integer == 0) {
			r = eval(((IfElse *) base)->fals);
		} else {
			r = eval(((IfElse *) base)->tru);
		}
		freeResult(&c);
		break;
	}
	case 'A': { /* sym definition */
		r->type = R_int;
		setsym(((SymbolAssign *)base)->s, ((SymbolAssign *)base)->l);
		r->integer = 0; /* signal success */
		break;
	}
	case 'G': { /* group node */
		r->type = R_group;
		r->group = ((Group *) base)->type;
		Result *larg = eval(((Group *) base)->l);
		r->integer = larg->integer;
		freeResult(&larg);
		// printf("Got type:%d value:%d\n", r->group, r->integer);
		if (((Group *) base)->r) {
			r->next = eval(((Group *) base)->r);
		}
		break;

	}
	default: {
		printf("unknown eval type, got %d",base->nodetype);
		exit(3);
	}
	}

	return r;
}
