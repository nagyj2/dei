/** Structs and functions to create and operate on ASTs.
 * Includes AST elements and support functions for creation, destruction and debugging.
 * @file ast.h
 * @author Jason Nagy (jaysun_n@hotmail.com)
 * @version 0.1
 * @date 2021-07-26
 * 
 * @copyright Copyright (c) 2021
 * 
 */

#ifndef AST_H_INCLUDED
#define AST_H_INCLUDED

#include <string.h> /* strdup */

#include "struct.h"

/* ===== DATA ===== */

/** Builtin functions.
 * The operations which can be performed on a dice roll before standard arithmetic operations.
 */
enum bifs {
	B_append = 1,						/**< Add values to a roll result */
	B_drop,									/**< Drop values from a roll result */
	B_count,								/**< Count the number of selected items */
	B_choose,								/**< Return the selected items */
	B_reroll								/**< Modify the value the selected items */
};

/** Function selectors.
 * Decides how one or more elements from a roll will be chosen for a function.
 */
enum sifs {
	S_all = -1,							/**< Plural - Select ALL possible values */
	S_high = -2,						/**< Single - Select the highest value */
	S_low = -3,							/**< Single - Select the lowest value */
	S_rand = -4,						/**< Single - Select a random value */
	S_unique = -5						/**< Plural - Select ALL unique values in the roll */
};

/** Function conditionals.
 * If the function result evaluates to true, the old value is replaced.
 */
enum cifs {
	C_none = 1,							/**< Perform the function unconditionally. Always true. */
	C_high,									/**< True if the new result is higher. False otherwise */
	C_low										/**< True if the new result is lower. False otherwise */
};

/** A natural die definition.
 * A natural die contains all numbers between two bounds, inclusive. Also tracks the number of times to roll.
 */
struct natdie {
	int nodetype;						/**< D */
	int count;							/**< Number of times to perform dice rolls */
	int min;								/**< Minimum face on the natural die */
	int max;								/**< Maximum face on the natural die */
};

/** An artificial die definition.
 * An artificial die has enumerated faces by the user which will be rolled. Also tracks the number of times to roll.
 */
struct setdie {
	int nodetype;						/**< d */
	int count;							/**< Number of times to perform dice rolls */
	struct value *faces;		/**< Faces on the die */
};

/** A natural integer.
 * A number constant to perform arithmetic with once all dice have been operated on and summed.
 */
struct natint {
	int nodetype;						/**< I */
	int integer;						/**< The integer value being represented */
};

/** Function arguments.
 * Stores the required arguments for a function.
 */
struct fargs {
	int nodetype;						/**< C */
	int fcount;							/**< Times to perform function */
	int seltype;						/**< Type of select. Must be from enum @ref sifs. */
	int scount;							/**< Times to perform selection algorithm */
	int cond;								/**< conditional to do operation. Must be from enum @ref cifs */
};

/** A set result.
 * Used for 'faked' rolls. Treated as if the enumerated numbers had been rolled by a die.
 * A die result generated through this manner will have no data on the faces included.
 */
struct setres {
	int nodetype;						/**< Q */
	struct value *out;			/**< Output to set roll to */
};


/** Generic AST Node.
 * \struct ast
 * A node which can have up to 2 children. Used for operations which only require operator and operand information.
 */
struct ast {
	int nodetype;						/**< Math operations, functions, comparisons, M, R, S, Z */
	struct ast *l;					/**< Left operand */
	struct ast *r;					/**< Right operand */
};


/* ===== FUNCTIONS ===== */

/** Create a new AST node.
 * @param[in]  nodetype The operator the node will do.
 * @param[in]  l    The left operand subtree.
 * @param[in]  r    The right operand subtree. Can be NULL.
 * @return          An AST node representing the operation. Cannot be NULL.
 */
struct ast *newAst(int nodetype, struct ast *l, struct ast *r);
/** Create a comparison AST node.
 * @param[in]  cmptype The comparison subtype.
 * @param[in]  body    The left operand subtree.
 * @param[in]  args    The right operand subtree.
 * @return         An AST node representing the operation. Cannot be NULL.
 */
struct ast *newCmp(int cmptype, struct ast *body, struct ast *args);
/** Create a new function AST node.
 * @param[in]  functype The function subtype. Must be from _bifs_.
 * @param[in]  body     The subtree which the function will be performed on.
 * @param[in]  args     Arguments for the function.
 * @return          An AST node represnting the function. Cannot be NULL.
 */
struct ast *newFunc(int functype, struct ast *body, struct ast *args);

/** Create a natural die definition leaf.
 * @param[in]  count The number of times to roll the die. Cannot be zero or less.
 * @param[in]  min   The minimum face on the die.
 * @param[in]  max   The maximum face on the die.
 * @return       An AST representing a natural die roll. Cannot be NULL.
 */
struct ast *newNatdie(int count, int min, int max);
/** Create an artificial die definition leaf.
 * @param[in]  count The number of times to roll the die. Cannot be zero or less.
 * @param[in]  faces The faces on the die. Cannot be NULL.
 * @return       An AST representing an artificial die roll. Cannot be NULL.
 */
struct ast *newSetdie(int count, struct value *faces);
/** Create a natural integer leaf.
 * @param[in]  integer The number to be represented.
 * @return         An AST node representing an integer. Cannot be NULL.
 */
struct ast *newNatint(int integer);
/** Create function arguments leaf
 * @param[in]  fcount  The number of times to perform the function. Cannot be zero or less.
 * @param[in]  seltype The selection type. Must be from _sifs_.
 * @param[in]  scount  The number of times to perform selection. Cannot be zero or less.
 * @param[in]  cond    The condition type on when to save the function result. Must be from _cifs_.
 * @return         An AST node representing function arguments. Cannot be NULL.
 */
struct ast *newFargs(int fcount, int seltype, int scount, int cond);
/** Create a artificial roll leaf.
 * @param[in]  out The numbers which were 'rolled'. Can be NULL.
 * @return     An AST node representing a fake roll. Cannot be NULL.
 */
struct ast *newSetres(struct value *out);


/* ===== MEMORY MANAGEMENT ===== */

/** Recursively free memory from an AST tree.
 * @param[in,out] root Root node of an AST to free.
 * @sideeffect root pointer will be set to NULL.
 */
void freeAst( struct ast **root );


/* ===== DEBUGGING ===== */

/** Recursively print the contents of an AST tree.
 * @param[in] root The root node to print.
 * @sideeffect A string representation of root will be displayed to stdout.
 */
void printAst(struct ast *root);



#endif /* AST_H_INCLUDED */
