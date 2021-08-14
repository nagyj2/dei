/** Declarations for an evaluation interface.
 * @file eval.h
 * @author Jason Nagy (jaysun_n@hotmail.com)
 * @version 1.0
 * @date 2021-07-26
 * 
 * @copyright Copyright (c) 2021
 * 
 */

#ifndef EVAL_H_INCLUDED
#define EVAL_H_INCLUDED

#include <stdbool.h> /* bool */

#include "ast.h"
#include "result.h"


/* ===== FUNCTIONS ===== */

/** Evaluates an AST to produce an output.
 * Evaluation does not modify the input nodes, allowing for better memory management.
 * @param[in]  base The current AST node to evaluate.
 * @return      The result of the @p base node's operation.
 */
Result *eval(AST *base);



#endif /* EVAL_H_INCLUDED */
