/** Structs and functions to create and operate on ASTs.
 * Includes AST elements and support functions for creation, destruction and debugging.
 * @file ast.h
 */

#ifndef AST_H_INCLUDED
#define AST_H_INCLUDED

#include <string.h> /* strdup */

#include "struct.h"

/* === DATA === */

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

/** A symbol reference.
 * A symbol (variable) reference. The referenced symbol is guarenteed present in the symbol table, but it may not be initialized.
 */
struct symcall {
	int nodetype;						/**< E */
	struct symbol *sym;			/**< Called symbol */
};

/** Generic AST Node.
 * A node which can have up to 2 children. Used for operations which only require operator and operand information.
 */
struct ast {
	int nodetype;						/**< Math operations, functions, comparisons, M, R, S, Z */
	struct ast *l;					/**< Left operand */
	struct ast *r;					/**< Right operand */
};

/** Symbol assignment.
 * A node which will assign an ast value to a symbol. The ast value is not evaluated when saving it.
 */
struct astAsgn {
	int nodetype;						/**< A */
	struct symbol *s;				/**< Symbol to assign value to */
	struct ast *l;					/**< Value to assign to symbol */
};


/* === FUNCTIONS === */

/** Create a new AST node.
 * @param  nodetype[in] The operator the node will do.
 * @param  l[in]    The left operand subtree.
 * @param  r[in]    The right operand subtree. Can be NULL.
 * @return          An AST node representing the operation. Cannot be NULL.
 */
struct ast *newAst(int nodetype, struct ast *l, struct ast *r);
/** Create a comparison AST node.
 * @param  cmptype[in] The comparison subtype.
 * @param  body[in]    The left operand subtree.
 * @param  args[in]    The right operand subtree.
 * @return         An AST node representing the operation. Cannot be NULL.
 */
struct ast *newCmp(int cmptype, struct ast *body, struct ast *args);
/** Create a new function AST node.
 * @param  functype[in] The function subtype. Must be from _bifs_.
 * @param  body[in]     The subtree which the function will be performed on.
 * @param  args[in]     Arguments for the function.
 * @return          An AST node represnting the function. Cannot be NULL.
 */
struct ast *newFunc(int functype, struct ast *body, struct ast *args);
/** Create a new variable assignment.
 * @param  sym[in] The variable symbol to assign to.
 * @param  def[in] The AST definition of the variable.
 * @return     An AST node representing the assignment. Cannot be NULL.
 */
struct ast *newAsgn(struct symbol *sym, struct ast *def);

/** Create a natural die definition leaf.
 * @param  count[in] The number of times to roll the die. Cannot be zero or less.
 * @param  min[in]   The minimum face on the die.
 * @param  max[in]   The maximum face on the die.
 * @return       An AST representing a natural die roll. Cannot be NULL.
 */
struct ast *newNatdie(int count, int min, int max);
/** Create an artificial die definition leaf.
 * @param  count[in] The number of times to roll the die. Cannot be zero or less.
 * @param  faces[in] The faces on the die. Cannot be NULL.
 * @return       An AST representing an artificial die roll. Cannot be NULL.
 */
struct ast *newSetdie(int count, struct value *faces);
/** Create a natural integer leaf.
 * @param  integer[in] The number to be represented.
 * @return         An AST node representing an integer. Cannot be NULL.
 */
struct ast *newNatint(int integer);
/** Create function arguments leaf
 * @param  fcount[in]  The number of times to perform the function. Cannot be zero or less.
 * @param  seltype[in] The selection type. Must be from _sifs_.
 * @param  scount[in]  The number of times to perform selection. Cannot be zero or less.
 * @param  cond[in]    The condition type on when to save the function result. Must be from _cifs_.
 * @return         An AST node representing function arguments. Cannot be NULL.
 */
struct ast *newFargs(int fcount, int seltype, int scount, int cond);
/** Create a artificial roll leaf.
 * @param  out[in] The numbers which were 'rolled'. Can be NULL.
 * @return     An AST node representing a fake roll. Cannot be NULL.
 */
struct ast *newSetres(struct value *out);
/** Create a symbol (variable) call leaf.
 * @param  sym[in] The symbol to reference. Cannot be NULL.
 * @return     An AST node representing a symbol call. Cannot be NULL.
 */
struct ast *newSymcall(struct symbol *sym);

/* = Symtab = */

/** The symbol table which stores variables. */
extern struct symbol symtab[NHASH];
/** Searches the symbol table for a name and returns its location.
 * If the symbol table cannot find an already created version of the symbol, it will create a new entry and return it.
 * @param[in]  s The name of the symbol to search for.
 * @return   A pointer to the storage location of the symbol within the symbol table.
 */
struct symbol *lookup(char *s);

/** Assign an AST structure to a symbol.
 * @param[in,out] name The symbol reference to assign a meaning to. Cannot be NULL.
 * @param[in] val  The definition to be assigned to name
 * @sideeffect name will have a new func value
 */
void setsym(struct symbol *name, struct ast *def);


/* === MEMORY MANAGEMENT === */

/** Recursively free memory from an AST tree.
 * @param root[in,out] Root node of an AST to free.
 * @sideeffect root pointer will be set to NULL.
 */
void freeAst( struct ast **root );

/** Frees the entire contents of the symbol table.
 * 
 */
void freeTable();

/** Frees a symbol and the value associated with it.
 * 
 * @param[in,out] sym the symbol to be free.
 * @sideeffect @p sym is set to NULL after execution.
 */
void freeSymbol(struct symbol **sym);


/* ======= DEBUGGING ======= */

/** Recursively print the contents of an AST tree.
 * @param root[in] The root node to print.
 * @sideeffect A string representation of root will be displayed to stdout.
 */
void printAst(struct ast *root);


#endif /* AST_H_INCLUDED */
