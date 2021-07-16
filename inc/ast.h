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

enum bifs {								/* Builtin functions */
	B_append = 1,
	B_drop,
	B_count,
	B_choose,
	B_reroll
};

enum sifs {								/* Selectors */
	S_high = 1,
	S_low,
	S_rand,
	S_unique
};

enum cifs {								/* Conditionals */
	C_none = 1,
	C_high,
	C_low
};

struct natdie {						/* LEAF: A natural die definition */
	int nodetype;						/* D */
	int count;							/*  */
	int min;								/*  */
	int max;								/*  */
};

struct setdie {						/* LEAF: An artificial die definition */
	int nodetype;						/* d */
	int count;							/*  */
	struct value *faces;		/*  */
};

struct natint {						/* LEAF: A natural integer */
	int nodetype;						/* I */
	int integer;						/*  */
};

struct fargs {						/* LEAF: Function arguments */
	int nodetype;						/* C */
	int fcount;							/* Times to perform function */
	int seltype;						/* Type of select */
	int scount;							/* Times to perform selection algorithm */
	int cond;								/* conditional to do operation */
};

struct setres {						/* LEAF: A set result */
	int nodetype;						/* Q */
	struct value *out;			/* Output to set roll to */
};

struct symcall {					/* LEAF: A symbol reference */
	int nodetype;						/* E */
	struct symbol *sym;			/* Called symbol */
};

struct ast {							/* NODE: Generic AST Node */
	int nodetype;						/*  */
	struct ast *l;					/* Left operand */
	struct ast *r;					/* Right operand */
};

struct astAsgn {					/* NODE: */
	int nodetype;						/* A */
	struct symbol *s;				/* Symbol to assign value to */
	struct ast *l;					/* Value to assign to symbol */
};


/* === FUNCTIONS === */

/* Create a new AST node */
struct ast *newAst(int nodetype, struct ast *l, struct ast *r);
/* Create a new compare node */
struct ast *newCmp(int cmptype, struct ast *l, struct ast *r);
/* Create a new function node */
struct ast *newFunc(int functype, struct ast *l, struct ast *r);
/* Create a new assignment node */
struct ast *newAsgn(struct symbol *sym, struct ast *def);

/* create a natural die definition leaf */
struct ast *newNatdie(int count, int min, int max);
/* create an artificial die definition leaf */
struct ast *newSetdie(int count, struct value *faces);
/* create a natural integer leaf */
struct ast *newNatint(int integer);
/* create function arguments leaf */
struct ast *newFargs(int fcount, int seltype, int scount, int cond);
/* create a artificial roll leaf */
struct ast *newSetres(struct value *out);
/* create a symbol call leaf */
struct ast *newSymcall(struct symbol *sym);

/* enter a symbol and definition into the table */
void setsym(struct symbol *name, struct ast *def);


/* === MEMORY MANAGEMENT === */

/* Recursively free memory from an AST tree */
void freeAst( struct ast **root );


/* ======= DEBUGGING ======= */

/* Recursively print the contents of an AST tree */
void printAst(struct ast *root);


#endif /* AST_H_INCLUDED */
