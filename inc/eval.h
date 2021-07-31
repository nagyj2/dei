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

#include "value.h"
#include "ast.h"


/* ===== FUNCTIONS ===== */

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
