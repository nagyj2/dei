/** Implementation of the AST result structure.
 * @file result.c
 * @author Jason Nagy (jaysun_n@hotmail.com)
 * @version 0.1
 * @date 2021-07-31
 * 
 * @copyright Copyright (c) 2021
 * 
 */

#include <stdlib.h>

#ifdef DEBUG
#include <assert.h>
#endif

#include "result.h"


/* ===== FUNCTIONS ===== */


/* ===== MEMORY MANAGEMENT ===== */

/**
 * Frees all pointers within @p res and sets their values to NULL.
 */
void freeResult(Result **res) {
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
