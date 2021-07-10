/* dei STRUCTS */
/* This file contains:
 * - Struct declarations for AST
 * - Explanation of structs
 * - Methods to operate on the structs
 * - essential lexer functions
 * TODO: unroll 'selector'
 */


#ifndef DEI_STRUCT_H_INCLUDED
#define DEI_STRUCT_H_INCLUDED

#include <stdlib.h> /* needed for malloc(), free() */
#include <stdbool.h> /* for booleans */
#include <stdarg.h> /* needed for yyerror */

#include "defines.h"


/* ====== DATA ====== */
/* Support Data
 * function type                 (bifs)
 * eval result type              (rifs)
 *
 * chainable values              (value)
 * symbol table reference        (symbol)
 * desc. of what is selected.    (selector)
 */

enum bifs {           /* built-in functions */
 B_drop = 1,
 B_append,
 B_choose,
 B_reroll,
 B_count
};

enum sifs {           /* selectors */
  S_all = -1,         /* selector for 'all available' */
  S_high = -2,        /* highest in dieroll */
  S_low = -3,         /* lowest in dieroll */
  S_rand = -4,        /* random roll in dieroll */
  S_unique = -5       /* all unique rolls in dieroll */
  /* int is achieved by substituting the sifs input with a positive number */
};


struct value {        /* a linked list of values */
 int v;               /* value */
 struct value *next;  /* next element in list */
};

struct symbol {       /* a user defined symbol */
 char *name;          /* identifier */
 struct ast *func;    /* meaning of symbol -> an ast */
};


/* ==== AST NODE ==== */
/* Node Operations
 * CMP + - * DIV % ^
 * & | && ||
 * M
 *
 * Node Types
 * OP ast     : operator for arbitrary binops and unops
 * R  ast     : operatior to roll natural die -> act like Q
 * r  ast     : operatior to roll special die -> act like Q
 * S  ast     : operatior to sum rolled dice -> act like I
 * D  natdie  : definition of a natural die
 * d  setdie  : definition of a special face die
 * Q  setres  : a set roll outcome set
 * F  funcall : call to a special function
 * I  natint  : individual integer
 * E  symcall : a call to the symbol table
 * A  symasgn : set a symbol to an ast
 *
 * Support Nodes (not AST nodes themselves, but are used in defining some)
 * V  value   : chainable values
 */

struct ast {          /* R, r, S: the base ast node */
 int nodetype;        /* OPs R r S */
 struct ast *l;       /* left/first operand (or onlu operand) */
 struct ast *r;       /* right/second operand -> nullable */
};

struct natdie {       /* D: a die defined by max and min */
 int nodetype;        /* D */
 int count;           /* number of rolls */
 int min;             /* minimum die face */
 int max;             /* maximum die face */
};

struct setdie {       /* d: a die defined by face values */
 int nodetype;        /* d */
 int count;           /* number of rolls */
 struct value *faces; /* faces of the die */
};

struct natint {       /* I: natural integer */
 int nodetype;        /* I */
 int integer;         /* integer value */
};

struct setres {       /* Q: a set roll outcome */
 int nodetype;        /* Q */
 struct value *faces; /* roll results */
};

struct funcall {      /* F: function call */
 int nodetype;        /* F */
 int functype;        /* function type -> bifs */
 int seltype;         /* selection type -> sifs */
 int fcount;          /* times to perform function */
 int scount;          /* times to perform select */
 struct ast *l;       /* astroll used */
};

struct symcall {      /* E: symbol call (in expression) */
 int nodetype;        /* E */
 struct symbol *s;    /* referenced symbol */
};

struct symasgn {      /* A: assign an ast to a symbol  */
   int nodetype;      /* A */
   struct symbol *s;  /* referenced symbol */
   struct ast *l;     /* the ast to assign to symbol */
 };


/* ==== FUNCTION ==== */

/* Create a new value struct. If 'prev' != NULL, it will occupy 'next' attr of new var */
struct value *newValue(int elem, struct value *prev);
/* Iterate over a value chain and count the number of elements */
int countValue(struct value *val);
/* Iterate over a value chain and sum up their values */
int sumValue(struct value *val);
/* Iterate over a value chain to determine if 'key' is within */
bool hasValue(int key, struct value *list);

/* R r S : create a new ast node */
struct ast *newast(int nodetype, struct ast *l, struct ast *r);
/* create a comparison type node */
struct ast *newcmp(int cmptype, struct ast *l, struct ast *r);
/* D : create a natural die definition node */
struct ast *newnatdie(int count, int min, int max);
/* d : create a artificial face die node */
struct ast *newsetdie(int count, struct value *faces);
/* Q : create a node to roll the contained die */
struct ast *newsetres(struct value *faces);
/* F : create a builtin function call node */
struct ast *newfunc(int functype, int seltype, int fcount, int scount, struct ast *l);
/* I : create a natural integer node */
struct ast *newnatint(int integer);
/* E : create a symbol (variable) call node */
struct ast *newref(struct symbol *s);
/* A : create symbol (variable) assignment node */
struct ast *newasgn(struct symbol *s, struct ast *defn);


/* delete and free ast */
void freeAst(struct ast *a);
/* free all elements of a value chain */
void freeValue(struct value *a);

#endif /* DEI_STRUCT_H_INCLUDED */
