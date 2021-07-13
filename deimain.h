/* dei MAIN */
/* This file contains:
 * - Main function
 * - Lexer utility functions
 * - Logging capabilities
 * - essential lexer functions
 */

#ifndef DEIMAIN_H_INCLUDED
#define DEIMAIN_H_INCLUDED

#include <stdlib.h>
#include <stdio.h> /* needed for FILE */
#include <assert.h> /* for assert */
#include <time.h> /* needed for time() */

#include "defines.h"
//#include "symboltable.h"
//#include "util.h"

/* start the parsing process */
int yyparse(void);
int yylex(void);

/* main function */
int main(int argc, char **argv);

#endif /* DEIMAIN_H_INCLUDED */
