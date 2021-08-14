/** Structs and functions to create and operate on ASTs.
 * Includes AST elements and support functions for creation, destruction and debugging.
 * @file ast.h
 * @author Jason Nagy (jaysun_n@hotmail.com)
 * @version 1.0
 * @date 2021-07-26
 * 
 * @copyright Copyright (c) 2021
 * 
 */

#ifndef AST_H_INCLUDED
#define AST_H_INCLUDED

#include <string.h> /* strdup */

#include "value.h"


/* ===== DATA ===== */

/** Enum representing the available builtin functions.
 * The operations which can be performed on a dice roll before standard arithmetic operations.
 */
enum bifs {
	B_append = 1,						/**< Add values to a roll result */
	B_drop,									/**< Drop values from a roll result */
	B_count,								/**< Count the number of selected items */
	B_choose,								/**< Return the selected items */
	B_reroll								/**< Modify the value the selected items */
};

/** @typedef BuiltinFunc
 * Shorthand for the bifs structure.
 */
typedef enum bifs BuiltinFunc;


/** Enum representing the available ways to select inputs for functions.
 * Decides how one or more elements from a roll will be chosen for a function.
 */
enum sifs {
	S_all = -1,							/**< Plural - Select ALL possible values */
	S_high = -2,						/**< Single - Select the highest value */
	S_low = -3,							/**< Single - Select the lowest value */
	S_rand = -4,						/**< Single - Select a random value */
	S_unique = -5						/**< Plural - Select ALL unique values in the roll */
};

/** @typedef Selector
 * Shorthand for the sifs structure.
 */
typedef enum sifs Selector;


/** Enum representing when to keep the result of a function call.
 * If the function result evaluates to true, the old value is replaced.
 */
enum cifs {
	C_none = 1,							/**< Perform the function unconditionally. Always true. */
	C_high,									/**< True if the new result is higher. False otherwise */
	C_low										/**< True if the new result is lower. False otherwise */
};

/** @typedef Conditionals
 * Shorthand for the cifs structure.
 */
typedef enum cifs Conditionals;


/** Enum representing the types of rolls.
 * When evaluating, the results of different types will not be combined.
 */
enum difs {
	D_none = -1,						/**< No type. */
	D_check,								/**< Skill check. */
	D_slashing,							/**< Slashing Damage. */
	D_piercing,							/**< Piercing Damage. */
	D_bludgeoning,					/**< Bludgeoning Damage. */
	D_poison,								/**< Poison Damage. */
	D_acid,									/**< Acid Damage. */
	D_fire,									/**< Fire Damage. */
	D_cold,									/**< Cold Damage. */
	D_radiant,							/**< Radiant Damage. */
	D_necrotic,							/**< Necrotic Damage. */
	D_lightning,						/**< Lightning Damage. */
	D_thunder,							/**< Thunder Damage. */
	D_force,								/**< Force Damage. */
	D_psychic								/**< Psychic Damage. */
};

/** @typedef GroupType
 * Shorthand for difs structure.
 */
typedef enum difs GroupType;


/** Generic Binary AST Node.
 * A node which can have up to 2 children. Used for operations which only require operator and operand information.
 */
struct ast {
	int nodetype;						/**< Math operations, functions, comparisons, M, R, S, Z */
	struct ast *l;					/**< Left operand */
	struct ast *r;					/**< Right operand */
};

/** @typedef AST
 * Shorthand for the bifs structure.
 */
typedef struct ast AST;


/** A natural die definition.
 * A natural die contains all numbers between two bounds, inclusive. Also tracks the number of times to roll.
 */
struct natdie {
	int nodetype;						/**< D */
	int count;							/**< Number of times to perform dice rolls */
	int min;								/**< Minimum face on the natural die */
	int max;								/**< Maximum face on the natural die */
};

/** @typedef NatDie
 * Shorthand for the natdie structure.
 */
typedef struct natdie NatDie;


/** An artificial die definition.
 * An artificial die has enumerated faces by the user which will be rolled. Also tracks the number of times to roll.
 */
struct setdie {
	int nodetype;						/**< d */
	int count;							/**< Number of times to perform dice rolls */
	ValueChain *faces;		/**< Faces on the die */
};

/** @typedef SetDie
 * Shorthand for the setdie structure.
 */
typedef struct setdie SetDie;


/** A natural integer.
 * A number constant to perform arithmetic with once all dice have been operated on and summed.
 */
struct natint {
	int nodetype;						/**< I */
	int integer;						/**< The integer value being represented */
};

/** @typedef NatInt
 * Shorthand for the natint structure.
 */
typedef struct natint NatInt;


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

/** @typedef FuncArgs
 * Shorthand for the fargs structure.
 */
typedef struct fargs FuncArgs;


/** A set result.
 * Used for 'faked' rolls. Treated as if the enumerated numbers had been rolled by a die.
 * A die result generated through this manner will have no data on the faces included.
 */
struct setres {
	int nodetype;						/**< Q */
	ValueChain *out;				/**< Output to set roll to */
};

/** @typedef SetRoll
 * Shorthand for the setres structure.
 */
typedef struct setres SetRoll;


/** A conditionally evaluated statement.
 * Used to test a condition and then evaluate an AST according to the result.
 */
struct ifast {
	int nodetype;						/**< F */
	AST *cond;							/**< Condition to determine which branch to execute. */
	AST *tru;								/**< Branch to execute on a non-zero condition. */
	AST *fals;							/**< Branch to execute on a zero condition. */
};

/** @typedef IfElse
 * Shorthand for the ifast structure.
 */
typedef struct ifast IfElse;

/** A group type for a roll.
 * Used to separate output rolls.
 */
struct roll_group {
	int nodetype;						/**< G */
	int type;								/**< Group type being represented. */
	AST *l;									/**< Group value for this tree. */
	AST *r;									/**< Link to the next group (if any). */
};

/** @typedef Group
 * Shorthand for roll_group structure.
 */
typedef struct roll_group Group;


/* ===== FUNCTIONS ===== */

/** Create a new AST node.
 * If memory cannot be allocated, the program displays an error and exits.
 * Inputs must passed by reference.
 * @param[in]  nodetype The operation the node will perform.
 * @param[in]  l    The left operand subtree. Cannot be NULL.
 * @param[in]  r    The right operand subtree. Can be NULL.
 * @return          An AST node representing the operation. Cannot be NULL.
 */
AST *newAst(int nodetype, AST *l, AST *r);

/** Create a comparison AST node.
 * If memory cannot be allocated, the program displays an error and exits.
 * Inputs must passed by reference.
 * @param[in]  cmptype The comparison subtype.
 * @param[in]  body    The left operand subtree.
 * @param[in]  args    The right operand subtree.
 * @return         An AST node representing the operation. Cannot be NULL.
 */
AST *newCmp(int cmptype, AST *body, AST *args);

/** Create a new function AST node.
 * If memory cannot be allocated, the program displays an error and exits.
 * Inputs must passed by reference.
 * @param[in]  functype The function subtype. Must be from _bifs_.
 * @param[in]  body     The subtree which the function will be performed on.
 * @param[in]  args     Arguments for the function.
 * @return          An AST node represnting the function. Cannot be NULL.
 */
AST *newFunc(int functype, AST *body, AST *args);

/** Create a natural die definition leaf.
 * If memory cannot be allocated, the program displays an error and exits.
 * @param[in]  count The number of times to roll the die. Cannot be zero or less.
 * @param[in]  min   The minimum face on the die.
 * @param[in]  max   The maximum face on the die.
 * @return       An AST representing a natural die roll. Cannot be NULL.
 */
AST *newNatdie(int count, int min, int max);

/** Create an artificial die definition leaf.
 * If memory cannot be allocated, the program displays an error and exits.
 * Inputs must passed by reference.
 * @param[in]  count The number of times to roll the die. Cannot be zero or less.
 * @param[in]  faces The faces on the die. Cannot be NULL.
 * @return       An AST representing an artificial die roll. Cannot be NULL.
 */
AST *newSetdie(int count, ValueChain *faces);

/** Create a natural integer leaf.
 * If memory cannot be allocated, the program displays an error and exits.
 * @param[in]  integer The number to be represented.
 * @return         An AST node representing an integer. Cannot be NULL.
 */
AST *newNatint(int integer);

/** Create function arguments leaf.
 * Contains the selection method, number of times for function and selection to compute and a keep-result conditiona;.
 * If memory cannot be allocated, the program displays an error and exits.
 * @param[in]  fcount  The number of times to perform the function. Cannot be zero or less.
 * @param[in]  seltype The selection type. Must be from _sifs_.
 * @param[in]  scount  The number of times to perform selection. Cannot be zero or less.
 * @param[in]  cond    The condition type on when to save the function result. Must be from _cifs_.
 * @return         An AST node representing function arguments. Cannot be NULL.
 */
AST *newFargs(int fcount, int seltype, int scount, int cond);

/** Create a artificial roll leaf.
 * If memory cannot be allocated, the program displays an error and exits.
 * Inputs must passed by reference.
 * @param[in]  out The numbers which were 'rolled'. Can be NULL.
 * @return     An AST node representing a fake roll. Cannot be NULL.
 */
AST *newSetres(ValueChain *out);

/** Create a conditional execution node.
 * If memory cannot be allocated, the program displays an error and exits.
 * Inputs must passed by reference.
 * @param[in]  cond The condition to evaluate and determine what should be evaluated. Cannot be NULL.
 * @param[in]  tru 	The AST to evaluate if @p cond is non-zero. Cannot be NULL.
 * @param[in]  fals The AST to evaluate if @p cond is zero. Cannot be NULL.
 * @return     An AST node representing a conditional execution. Cannot be NULL.
 */
AST *newIfelse(AST *cond, AST *tru, AST *fals);

/** Create a new roll group.
 * Used to separate input rolls and differentiate the current rolls occuring.
 * @param[in] type 		The group type to be calculated. Must be from @ref GroupType.
 * @param[in] tree 		The AST representing the group being saved.
 * @param[in] next		The next group. Can be NULL.
 * @return AST* 			An AST node representing a distinct group.
 */
AST *newGroup(int type, AST *tree, AST *next);


/* ===== MEMORY MANAGEMENT ===== */

/** Frees memory allocated to an @ref AST tree.
 * Frees memory allocated to itself and all other child nodes.
 * @param[in,out] root Root node of an AST to free.
 * @sideeffect @p root pointer will be set to NULL, along with all nodes along the way.
 */
void freeAst( AST **root );



/* ===== DEBUGGING ===== */

/** Display a the string representation of the input group type to standard out.
 * @param[in] group A valid @ref GroupType.
 * @sideeffect The string version of @p group is displayed to standard out.
 */
void printGroup(int group);

/** Recursively print the contents of an @ref AST tree.
 * @param[in] root The root node to print.
 * @sideeffect A string representation of root will be displayed to stdout.
 */
void printAst(AST *root);





#endif /* AST_H_INCLUDED */
