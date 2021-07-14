/* Dei Support Structures and Functions
 * Contents:
 * General Structs
 * Functions to manipulate Structs
 */

#ifndef STRUCT_H_INCLUDED
#define STRUCT_H_INCLUDED

#define NHASH 9997

#include <stdbool.h> /* bool */


/* === DATA === */

struct value {									/*  */
	int i;												/*  */
	struct value *next;						/*  */
};

struct symbol {       					/*  */
 char *name;          					/*  */
 struct ast *func;    					/*  */
};

struct symbol symtab[NHASH];		/* symbol table itself */


/* === FUNCTIONS === */

/* = Value = */

/* create new single value or append a chain */
struct value *newValue(int i, struct value *prev);
/* copy a value chain */
struct value *dupValue(struct value *base);
/* return the back of a chain */
struct value *backValue(struct value *base);
/* return the first struct containing key  */
struct value *findValue(int key, struct value *base);
/* remove key IF it is in base and return it */
struct value *popValue(int key, struct value **base);
/* remove key IF it is in base and return it */
struct value *removeValue(struct value *key, struct value **base);

/* copy a single value */
struct value *copyValue(struct value *base);

/* return number of elements in base */
int countValue(struct value *base);
/* return sum elements of base  */
int sumValue(struct value *base);
/* return whether exact value is in base */
bool hasValueExact(struct value *key, struct value *base);
/* return whether int value is in base */
bool hasValue(int key, struct value *base);

/* = Symtab = */

/* create a new symbol table */
void newSymtab(struct symbol result[]);
/* return spot in symtab : NOTE only initializes name! */
struct symbol *lookup(struct symbol table[], char *s);


/* === MEMORY MANAGEMENT === */

/* free a value chain */
void freeValue( struct value **val );

/* free the entire symbol table */
//void freeTable( struct symbol *table[] );

#endif /* STRUCT_H_INCLUDED */
