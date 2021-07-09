/* dei SYMBOL TABLE */
/* This file contains:
 * - function declarations regarding the symbol table
 */

#ifndef DEI_STRUCT
#include "struct.h"
#endif

/* -------------- SYMBOL TABLE -------------- */

#define NHASH 9997 /* size of symbol table */
struct symbol symtab[NHASH];    /* symbol table itself */
struct symbol *lookup(char *s); /* looks up a string in the symbol table and returns the entry */

void printsymtab(void); /* print entries in the table */
void freesymboltable(void); /* free the entire symbol table */
