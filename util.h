/* dei UTILITIES */
/* This file contains:
 * - utility data
 * - utility function declarations
 * NOTE: These are not strictly required for the production version
 */
 
#ifndef DEI_STRUCT
#include "struct.h"
#endif

/* print the ast to the screen */
void printtree(struct ast *a);

/* print a chain of values */
void printvalue(struct value *a);
