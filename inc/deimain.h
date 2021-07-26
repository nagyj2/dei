/** The main entrypoint for the Dei program.
 * Performs setup functions required for the program to work.
 * @file deimain.h
 * @author Jason Nagy (jaysun_n@hotmail.com)
 * @version 0.1
 * @date 2021-07-26
 * 
 * @copyright Copyright (c) 2021
 * 
 */

#ifndef DEIMAIN_H_INCLUDED
#define DEIMAIN_H_INCLUDED


#include <stdio.h> /* needed for FILE */

extern int yydebug;         /**< Bison debug flag */
extern FILE *yyin;          /**< Pointer to input stream */
extern int yylineno; 				/**< Current parsing line number in input stream. */

/** Restarts parsing internal state for a new input source.
 * @param file The new input stream
 */
extern void yyrestart(FILE* file);
/** Output a formatted string and produce an error token in the parser.
 * @param s       A format string.
 * @param VARARGS Elements to insert into the format string.
 * @sideefect 		The input formatted string is displayed to stderr and a 'error' token will be inserted into the parsing state.
 */
void yyerror(char *s, ...);
/** Starts the parsing process on yyin.
 * @return An exit value depending on whether the parse was successful.
 */
int yyparse();
/** Starts the lexing process on yyin.
 * @return An exit value depending on whether the lex was successful.
 */
int yylex();

/** The main entry point of the program.
 * Performs setup for file parsing before taking from stdin.
 * @param  argc The number of arguments in argv.
 * @param  argv Any CLI arguments.
 * @return      An exit value depending on whether the program ended successfully.
 */
int main(int argc, char **argv);


#endif /* DEIMAIN_H_INCLUDED */
