/** Contains flags and markers used by the parser.
 * @file flags.h
 * @author Jason Nagy (jaysun_n@hotmail.com)
 * @version 0.1
 * @date 2021-08-03
 * 
 * @copyright Copyright (c) 2021
 * 
 */

#include "ast.h"

/**
 * Enum to determine the type of statement which was executed.
 */
enum stmt_type {
	O_math = 1,			/**< Computation output was standard math. */
	O_assign,				/**< Computation output was an assignment. */
	O_none					/**< Computation output was blank. */
};

/**
 * @typedef Type
 * Shorthand for stmt_type.
 */
typedef enum stmt_type Type;


/**
 * The output type of a statement
 */
struct stmt_result {
	int type;				/**< Type of the result. Must be from @ref Type. */
	int flag;				/**< Flag for special meanings associated with type. */
	AST *ast;				/**< The statement result @ref AST. */
};

/**
 * @typedef State
 * Shorthand for stmt_result.
 */
typedef struct stmt_result State;


/* ===== FUNCTIONS ===== */

State *newState(int type, AST *ast);


/* ===== MEMORY MANAGEMENT ===== */

void freeState(State **state);
