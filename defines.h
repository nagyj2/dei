/* dei DEFINES
 * This file contains:
 * Definitions useful to multiple other files
 *
 * NOTE: This is a header only file
 */

#ifndef DEFINES_H_INCLUDED
#define DEFINES_H_INCLUDED

#include <stdio.h> /* needed for FILE */

/* One line log statement for debug printing and logging */
/* debug_state -> outputs a string without formatting or args
 * debug_report -> outputs a string with formatting
 * debug_log -> outputs a string with formatting to the log file
 */
#ifdef DEBUG
#define debug_state(fmt) \
  fprintf(stderr, fmt)
#define debug_report(fmt, ...) \
  do { fprintf(stderr, fmt, __VA_ARGS__); } while (0)
#define debug_log(fmt, ...) \
  do { fprintf(stderr, logger, __VA_ARGS__); } while (0)
#else
#define debug_state(fmt, ...)
#define debug_report(fmt, ...)
#define debug_log(fmt, ...)
#endif

#define NHASH 9997 /* size of symbol table */


/* === LEXER === */
extern int yydebug;         /* debug flag */
extern FILE *yyin;          /* input stream */
extern int yylineno; /* current lexer line */
extern void yyerror(char *s, ...);

FILE *logger;

#endif /* DEFINES_H_INCLUDED */
