/** Implementation of the statement result structure functions.
 * @file flags.c
 * @author Jason Nagy (jaysun_n@hotmail.com)
 * @version 1.0
 * @date 2021-08-03
 * 
 * @copyright Copyright (c) 2021
 * 
 */

#include "stdlib.h" /* malloc, free */
#include "stdio.h" /* printf */

#include "flags.h"
#include "symbols.h"


State *newState(int type, AST *ast) {
	State *a = malloc(sizeof(State));

	if (!a){
		printf("out of space\n");
		exit(1);
	}

	a->type = type;
	a->flag = 0;
	a->ast = ast;
	return a;
}

void freeState(State **state) {
	free(*state);
	*state = NULL;
}

// void freeStateComplete(State **state) {

// 	switch ((*state)->type) {
// 	case O_assign:
// 	case O_math:
// #ifdef SYMBOLS_H_INCLUDED
// 		freeAst_Symbol(&((*state)->ast));
// #else
// 		freeAst(&((*state)->ast));
// #endif
// 		break;

// 	case O_none:

// 	}

// 	free(*state);
// 	*state = NULL;
// }
