/* dei EVALUATION FUNCTIONS */
/* This file contains:
 * - functions for ast creation
 * - functions for ast evaluation
 */

/*
#define YYDEBUG
*/

#ifndef DEI_STRUCT
#include "struct.h"
#endif


/* ---------------- FUNCTION ---------------- */

/* build an ast */
struct ast *newast(int nodetype, struct ast *l, struct ast *r); /* op node */
struct ast *newcmp(int cmptype, struct ast *l, struct ast *r);  /* cmp op node */

struct ast *newnatdie(int count, int min, int max);             /* create a new natural die */
struct ast *newsetdie(int count, struct value *faces);          /* create a new special die */

struct ast *newsetres(struct value *faces);                     /* create a new roll definition */
                                                                /* function call */
struct ast *newfunc(int functype, struct selector *selectype, int times, struct ast *operand);
struct ast *newnatint(int integer);                             /* create an integer */

struct ast *newref(struct symbol *s);                           /* symbol call */
struct ast *newasgn(struct symbol *s, struct ast *meaning);     /* symbol assignment */

struct value *newvalue(int i, struct value *val);               /* create a value or chain them together */
struct selector *newselector(int sel, int count);

/* set the value of a symbol - save the ast */
void setsym(struct symbol *name, struct ast *val);

/* evaluate ast */
struct result *eval(struct ast *a);

/* perform a function on a function call */
struct result *callbuiltin(struct result *output, int functype, struct selector *sel, struct ast *frame);
