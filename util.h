/* dei UTILITIES */
/* This file contains:
 * - utility data
 * - utility function declarations
 * NOTE: These are not strictly required for the production version
 */

#ifndef DEI_UTIL_H_INCLUDED
#define DEI_UTIL_H_INCLUDED

#include <stdlib.h> /* needed for rand() */
#include <time.h> /* needed for time() */

#include "struct.h"


/* ==== FUNCTION ==== */

/* Picks a random number between 'min' and 'max', inclusive */
int randint(int min, int max);
/* Picks a random number from an option within 'faces' */
int randroll(struct value *faces);

/* print the ast to the screen */
void printtree(struct ast *a);
/* print a chain of values */
void printvalue(struct value *a);
/* print the contents of a symbol table */
void printsymtab(void);

#endif /* DEI_UTIL_H_INCLUDED */
