/* dei EVALUATION FUNCTIONS */
/* This file contains:
 * - functions for ast creation
 * - functions for ast evaluation
 */

#ifndef DEI_EVALUATION_H_INCLUDED
#define DEI_EVALUATION_H_INCLUDED

#include <stdlib.h> /* needed for malloc(), free() */
#include <math.h> /* needed for pow() */
#include <assert.h> /* needed for assert() */

#include "defines.h"
#include "struct.h"

/* ====== DATA ====== */
/* Support Data
 * eval result type              (rifs)
 *
 * eval rep. of a die            (die)
 * eval rep. of a die roll       (roll)
 * eval rep. of a result         (result)
 * contains a list of value ref. (selected)
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

/*struct selector { */      /* multiselect  */
/* int sel;     */         /* what to select -> sifs */
/* int count;  */          /* number of times to perform */
/*};*/


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
/* Place elem's of 'sel2' into 'sel1' if they aren't already there */
/* 'sel2' is safe to be freed after usage. Number of merges are returned */
int mergeSelected(struct selected **sel1, struct selected *sel2);
/* Look through 'opts' and return the first not in 'has' */
/* CAUTION : Can return NULL */
struct value *firstunique(struct value **opts, struct selected *has);


/* Create a die's face values from the max and min values */
struct value *createNatdieFace(int min, int max);
/* Create a die's face values from explicit options */
struct value *createSetdieFace(struct value *faces);


/* Saves a symbol to the AST */
void setsym(struct symbol *name, struct ast *val);


/* Recursively evaluate an AST to retrieve a result */
struct result *eval(struct ast *a);
/* Perform a builtin function to an evaluated AST result */
struct result *callbuiltin(int functype, int seltype, int fcount, int scount, struct ast *frame);
/* Rerandomizes all pointers within 'sel' according to the options of 'faces' */
/* NEITHER sel or faces can be NULL */
void funcreroll(struct selected *sel, struct value *faces);
/* Removes the pointers contained within 'sel' from 'out' */
void funcdrop(struct selected *sel, struct value *out);
/* Append selected point from 'sel' onto 'out' */
void funcappend(struct selected *sel, struct value *out);
/* Return elements of 'sel' */
void funcchoose(struct selected *sel, struct value *out);
/* Count the number of elements in 'sel' and place it in 'out' */
void funccount(struct selected *sel, struct value **out);


/* Create a new selector. 'seltype' is the type of select and 'count' is the number of selections */
/*struct selector *newSelector(int seltype, int count);*/
/* Returns selected roll results according to input selection data */
struct selected *select(int seltype, int scount, struct roll *dieroll);


/* free result variable, AND contained data - SETS ARGS TO NULL */
void freeResult(struct result **a);
/* free result variable, but leaves connected non-eval components - SETS ARGS TO NULL */
void freeResultSafe(struct result **a);
/* free selected nodes, but NOT the value nodes they hold */
void freeSelected(struct selected **a);



#endif /* DEI_EVALUATION_H_INCLUDED */
