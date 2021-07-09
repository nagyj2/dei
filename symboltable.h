/* dei SYMBOL TABLE */
/* This file contains:
 * - function declarations regarding the symbol table
 */

#ifndef SYMBOLTABLE_H_INCLUDED
#define SYMBOLTABLE_H_INCLUDED

#include <string.h> /* needed for strdup() */

#include "struct.h"

/* === DATA === */

#define NHASH 9997 /* size of symbol table */
struct symbol symtab[NHASH];    /* symbol table itself */

/* === FUNCTIONS === */

struct symbol *lookup(char *s); /* looks up a string in the symbol table and returns the entry */


/* === MEMORY MANAGEMENT === */

void freesymboltable(void); /* free the entire symbol table */

#endif /* SYMBOLTABLE_H_INCLUDED */
