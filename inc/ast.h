/* Dei AST Structures and Functions
 * Contents:
 * AST Structs
 * Functions to manipulate AST Structs
 */

#ifndef AST_H_INCLUDED
#define AST_H_INCLUDED


#include "struct.h"

/* AST Structure
 * Nodes
 * ast:			+ - * DIV % ^ & | INTER UNION
 * 					M
 * 	(ineq)	1 2 3 4 5 6
 * 	(func)	e f g h i
 * 					R - roll a die
 * 					r - fake a roll
 * 					S - sum a die roll
 * 					Z - strip information about dice (set faces = NULL)
 * astAsgn:	A - assign a value to a symbol
 *
 * Leafs
 * natdie:	D - A natural die
 * setdie:	d - An artificial die
 * natint:	I - A number literal
 * fargs:		C - selector
 * setres:	Q - An artificial roll
 * symcall:	E - a symbol reference
 */

/* === DATA === */

enum bifs {								/**< Builtin functions */
	B_append = 1,						/**< Add values to a roll result */
	B_drop,									/**< Drop values from a roll result */
	B_count,								/**< Count the number of selected items */
	B_choose,								/**< Return the selected items */
	B_reroll								/**< Modify the value the selected items */
};

enum sifs {								/**< Function selectors */
	S_high = 1,							/**< Single - Select the highest value */
	S_low,									/**< Single - Select the lowest value */
	S_rand,									/**< Single - Select a random value */
	S_unique								/**< Plural - Select ALL unique values in the roll */
};

enum cifs {								/**< Function conditionals */
	C_none = 1,							/**< Perform the function unconditionally */
	C_high,									/**< Perform the function if result is higher */
	C_low										/**< Perform the function if result is lower */
};

struct natdie {						/**< LEAF: A natural die definition */
	int nodetype;						/**< D */
	int count;							/**< Number of times to perform dice rolls */
	int min;								/**< Minimum face on the natural die */
	int max;								/**< Maximum face on the natural die */
};

struct setdie {						/**< LEAF: An artificial die definition */
	int nodetype;						/**< d */
	int count;							/**< Number of times to perform dice rolls */
	struct value *faces;		/**< Faces on the die */
};

struct natint {						/**< LEAF: A natural integer */
	int nodetype;						/**< I */
	int integer;						/**< The integer value being represented */
};

struct fargs {						/**< LEAF: Function arguments */
	int nodetype;						/**< C */
	int fcount;							/**< Times to perform function */
	int seltype;						/**< Type of select */
	int scount;							/**< Times to perform selection algorithm */
	int cond;								/**< conditional to do operation */
};

struct setres {						/**< LEAF: A set result */
	int nodetype;						/**< Q */
	struct value *out;			/**< Output to set roll to */
};

struct symcall {					/**< LEAF: A symbol reference */
	int nodetype;						/**< E */
	struct symbol *sym;			/**< Called symbol */
};

struct ast {							/**< NODE: Generic AST Node */
	int nodetype;						/**< Math operations, functions, comparisons, M, R, S, Z */
	struct ast *l;					/**< Left operand */
	struct ast *r;					/**< Right operand */
};

struct astAsgn {					/**< NODE: */
	int nodetype;						/**< A */
	struct symbol *s;				/**< Symbol to assign value to */
	struct ast *l;					/**< Value to assign to symbol */
};


/* === FUNCTIONS === */

/** Create a new AST node
 * @param  nodetype The operator the node will do
 * @param  l        The left operand subtree
 * @param  r        The right operand subtree. Can be NULL
 * @return          An AST node representing the operation. Cannot be NULL
 */
struct ast *newAst(int nodetype, struct ast *l, struct ast *r);
/** Create a comparison AST node
 * @param  cmptype The comparison subtype
 * @param  l       The left operand subtree
 * @param  r       The right operand subtree
 * @return         An AST node representing the operation. Cannot be NULL
 */
struct ast *newCmp(int cmptype, struct ast *body, struct ast *args);
/** Create a new function AST node
 * @param  functype The function subtype. Must be from _bifs_
 * @param  body     The subtree which the function will be performed on
 * @param  args     Arguments for the function
 * @return          An AST node represnting the function. Cannot be NULL
 */
struct ast *newFunc(int functype, struct ast *body, struct ast *args);
/** Create a new variable assignment
 * @param  sym The variable symbol to assign to
 * @param  def The AST definition of the variable
 * @return     An AST node representing the assignment. Cannot be NULL
 */
struct ast *newAsgn(struct symbol *sym, struct ast *def);

/** Create a natural die definition leaf
 * @param  count The number of times to roll the die. Cannot be zero or less
 * @param  min   The minimum face on the die
 * @param  max   The maximum face on the die
 * @return       An AST representing a natural die roll. Cannot be NULL
 */
struct ast *newNatdie(int count, int min, int max);
/** Create an artificial die definition leaf
 * @param  count The number of times to roll the die. Cannot be zero or less
 * @param  faces The faces on the die. Cannot be NULL
 * @return       An AST representing an artificial die roll. Cannot be NULL
 */
struct ast *newSetdie(int count, struct value *faces);
/** Create a natural integer leaf
 * @param  integer The number to be represented
 * @return         An AST node representing an integer. Cannot be NULL
 */
struct ast *newNatint(int integer);
/** Create function arguments leaf
 * @param  fcount  The number of times to perform the function. Cannot be zero or less
 * @param  seltype The selection type. Must be from _sifs_
 * @param  scount  The number of times to perform selection. Cannot be zero or less
 * @param  cond    The condition type on when to save the function result. Must be from _cifs_
 * @return         An AST node representing function arguments. Cannot be NULL
 */
struct ast *newFargs(int fcount, int seltype, int scount, int cond);
/** Create a artificial roll leaf
 * @param  out The numbers which were 'rolled'. Can be NULL
 * @return     An AST node representing a fake roll. Cannot be NULL
 */
struct ast *newSetres(struct value *out);
/**
 * Create a symbol (variable) call leaf
 * @param  sym The symbol to reference. Cannot be NULL
 * @return     An AST node representing a symbol call. Cannot be NULL
 */
struct ast *newSymcall(struct symbol *sym);

/** Set the definition of {@code name} to {@code def}.
 *
 * @param name Pointer to the symbol to save. Cannot be NULL
 * @param def  What the symbol will represent. Cannot be NULL
 */
void setsym(struct symbol *name, struct ast *def);


/* === MEMORY MANAGEMENT === */

/** Recursively free memory from an AST tree
 * @param root Root node of an AST to free
 * @sideeffect {@code root} pointer will be set to NULL
 */
void freeAst( struct ast **root );


/* ======= DEBUGGING ======= */

/** Recursively print the contents of an AST tree
 * @param root The root node to print
 */
void printAst(struct ast *root);


#endif /* AST_H_INCLUDED */
