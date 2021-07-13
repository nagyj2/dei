/* dei UTILITIES */
/* This file contains:
 * - utility data
 * - utility function declarations
 * NOTE: These are not strictly required for the production version
 */

#ifndef DEI_UTIL_H_INCLUDED
#define DEI_UTIL_H_INCLUDED

#include <stdlib.h> /* needed for malloc(), free(), rand() */
#include <time.h> /* needed for time() */

#include "defines.h"
#include "struct.h"


/* ==== FUNCTION ==== */

/* Picks a random number between 'min' and 'max', inclusive */
int randint(int min, int max);
/* Picks a random number from an option within 'faces' */
int randroll(struct value *faces);

/* print the ast to the screen */
void printAst(struct ast *a);
/* print a chain of values */
void printValue(struct value *a);
/* print the contents of a symbol table */
void printSymtab(struct symbol symtab[]);

#endif /* DEI_UTIL_H_INCLUDED */
