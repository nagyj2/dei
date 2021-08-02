/** Contains @ref Result structure, an evaluated AST result. Has ability to hold multiple data types.
 * Also contains declarations for functions to manipulate these structures.
 * @file result.h
 * @author Jason Nagy (jaysun_n@hotmail.com)
 * @version 0.1
 * @date 2021-07-31
 * 
 * @copyright Copyright (c) 2021
 * 
 */

#ifndef RESULT_H_INCLUDED
#define RESULT_H_INCLUDED

#include "value.h"


/* ===== DATA ===== */

/** The type of result from the evaluation of a specific AST node.
 * The type of node being used will imply which fields of the result are filled out.
 */
enum rifs {
	R_die = 1,						/**< Die definition. */
	R_roll,								/**< Roll result with die face information. */
	R_set,								/**< Roll result without die information. */
	R_int									/**< Evaluated integer. */
};

/** @typedef ResultType
 * Shorthand for the rifs structure.
 */
typedef enum rifs ResultType;


/** The result type used for evaluation.
 * The result type used type to determine which other field will be used for tracking the result.
 * All elements, except for the val state variable of sel, are completely separate
 */
struct result {
	int type;							/**< The type of result. Determines which field will be populated. Depends on production being evaluated. Must be from @ref rifs */
	ValueChain *faces;		/**< If type is R_die or R_roll, this represents the faces on the die. Can be NULL for R_roll. Otherwise it is NULL.  */
	ValueChain *out;			/**< If type is R_roll or R_set, this represents the actual rolled numbers. Otherwise it is NULL. */
	int integer;					/**< If type is R_int Otherwise it is NULL. */
};

/** @typedef Result
 * Shorthand for the ast_result structure.
 */
typedef struct result Result;



/* ===== FUNCTIONS ===== */

/** Tests a result struct to ensure relevant fields are filled and others are not.
 * @param[in] res The result struct to be tested.
 * @deprecated Never used.
 */
void ensureType(Result *res);



/* ===== MEMORY MANAGEMENT ===== */

/** Frees memory allocated to a @ref Result and its properties.
 * @param[in,out] res The @ref Result to be freed.
 * @sideeffect All pointers contained within @p res will also be freed, affecting aliased variables.
 */
void freeResult(Result **res);


#endif /* RESULT_H_INCLUDED */
