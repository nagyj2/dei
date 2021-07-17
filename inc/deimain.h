/**
 * @file deimain.h
 */

#ifndef DEIMAIN_H_INCLUDED
#define DEIMAIN_H_INCLUDED

#include <stdlib.h>
#include <stdio.h> /* needed for FILE */
#include "struct.h"

extern int yydebug;         /* debug flag */
extern FILE *yyin;          /* input stream */
extern int yylineno; /* current lexer line */
extern void yyrestart(FILE*);

void yyerror(char *s, ...);
int yyparse();
int yylex();

/* entry function */
int main(int argc, char **argv);
/* lexer and parser error function */
void yyerror(char *s, ...);

#endif /* DEIMAIN_H_INCLUDED */
