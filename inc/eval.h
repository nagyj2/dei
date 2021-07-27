/** Declarations for an evaluation interface.
 * @file eval.h
 * @author Jason Nagy (jaysun_n@hotmail.com)
 * @version 0.1
 * @date 2021-07-26
 * 
 * @copyright Copyright (c) 2021
 * 
 */

#ifndef EVAL_H_INCLUDED
#define EVAL_H_INCLUDED

#include <stdbool.h> /* bool */

#include "struct.h"
#include "ast.h"


/* ===== DATA ===== */

/** The type of resultfrom the evaluation of a specific AST node.
 * The type of node being used will imply which fields of the result are filled out.
 */
enum rifs {
	R_die = 1,
	R_roll,
	R_set,
	R_int
};



/** The result type used for evaluation.
 * The result type used type to determine which other field will be used for tracking the result.
 * All elements, except for the val state variable of sel, are completely separate
 */
struct result {
	int type;								/**< The type of result. Determines which field will be populated. Depends on production being evaluated. Must be from @ref rifs */
	ValueChain *faces;		/**< If type is R_die or R_roll, this represents the faces on the die. Can be NULL for R_roll. Otherwise it is NULL.  */
	ValueChain *out;			/**< If type is R_roll or R_set, this represents the actual rolled numbers. Otherwise it is NULL. */
	int integer;						/**< If type is R_int Otherwise it is NULL. */
};


/* ===== FUNCTIONS ===== */

/** Tests a result struct to ensure relevant fields are filled and others are not.
 * @param[in] res The result struct to be tested.
 */
void ensureType(struct result *res);


/* ===== EVALUATION ===== */

/** Evaluates an AST to produce an output.
 * Evaluation does not modify the input nodes, allowing for better memory management.
 * @param[in]  base The current AST node to evaluate.
 * @return      The result of the @p base node's operation.
 */
struct result *eval(struct ast *base);


/* ===== MEMORY MANAGEMENT ===== */

/** Frees memory allocated to input res according to its node type.
 * @param[in,out] res The result node to free.
 * @sideeffect The input's memory location and all those subsequently pointed to will be freed and the pointers set to NULL.
 */
void freeResult(struct result **res);



#endif /* EVAL_H_INCLUDED */
