/* dei MEMORY */
/* This file contains:
 * - function declarations for memory management
 */

#ifndef DEI_STRUCT
#include "struct.h"
#endif

/* delete and free ast */
void treefree(struct ast *a);

/* free all elements of a value chain */
void valuefree(struct value *a);

/* free result variable */
void resultfree(struct result *a);

/* free selected nodes, but NOT the value nodes they hold */
void selectedfree(struct selected *a);
