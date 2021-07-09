/* dei EVALUATION FUNCTIONS */
/* This file contains:
 * - functions for ast creation
 * - functions for ast evaluation
 */

#ifndef DEI_EVALUATION_H_INCLUDED
#define DEI_EVALUATION_H_INCLUDED

#include <math.h> /* needed for pow() */

#include "dei.tab.h"
#include "struct.h"

/* ====== DATA ====== */
/* Support Data
 * eval result type              (rifs)
 *
 * eval rep. of a die            (die)
 * eval rep. of a die roll       (roll)
 * eval rep. of a result         (result)
 * contains a list of value ref. (selected)
 * desc. of what is selected.    (selector)
 */

enum rifs {             /* result type */
 R_int = 1,            /* an integer result */
 R_roll,               /* a set of rolls and the die used */
 R_die                 /* a die with faces and times to roll */
};

struct die {            /* result for a die defintion */
 int count;            /* number of rolls */
 struct value *faces;  /* die faces */
};

struct roll {           /* result for a die roll */
 struct value *out;    /* roll result */
 struct value *faces;  /* used die - WARNING : NULLABLE */
};

struct result {         /* result value of */
 int type;             /* type of result -> rifs */
 int i;                /* holder for integer */
 struct roll *r;       /* holder for roll result, die used */
 struct die *d;        /* holder for die faces, rolls required */
};

struct selected {       /* a chain of pointers to assorted value nodes */
 struct value *val;      /* selected node */
 struct selected *next;  /* next selected node */
};

struct selector {       /* multiselect  */
 int sel;              /* what to select -> sifs */
 int count;            /* number of times to perform */
};


/* Created a new selected struct. If 'prev' != NULL, it will occupy the 'next' attr of the new var */
struct selected *newSelected(struct value *elem, struct selected *prev);
/* Iterate over a selected chain and count the number of elements */
int countSelected(struct selected *sel);
/* Iterate over a selected chain and sum the contents of the values at each node */
int sumSelected(struct selected *sel);
/* Iterate over a selected chain and determine if a VALUE pointer is contained */
bool hasSelected(struct value *key, struct selected *sel);
/* Iterate over a selected chain and determine if a SPECIFIC INTEGER is contained */
bool hasSelectedInt(int key, struct selected *sel);


/* Create a die's face values from the max and min values */
struct value *createNatdieFace(int min, int max);
/* Create a die's face values from explicit options */
struct value *createSetdieFace(struct value *faces);


/* Saves a symbol to the AST */
void setsym(struct symbol *name, struct ast *val);


/* Recursively evaluate an AST to retrieve a result */
struct result *eval(struct ast *a);
/* Perform a builtin function to an evaluated AST result */
struct result *callbuiltin(struct result *output, int functype, struct selector *sel, struct ast *frame);


/* Create a new selector. 'seltype' is the type of select and 'count' is the number of selections */
struct selector *newSelector(int seltype, int count);


/* free result variable */
void freeResult(struct result *a);
/* free selected nodes, but NOT the value nodes they hold */
void freeSelected(struct selected *a);

#endif /* DEI_EVALUATION_H_INCLUDED */
