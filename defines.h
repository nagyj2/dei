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
/* DEBUG_STATE -> outputs a string without formatting or args
 * DEBUG_REPORT -> outputs a string with formatting
 * DEBUG_LOG -> outputs a string with formatting to the log file
 */
#ifdef DEBUG
#define DEBUG_STATE(fmt) \
  fprintf(stderr, fmt)
#define DEBUG_REPORT(fmt, ...) \
  do { fprintf(stderr, fmt, __VA_ARGS__); } while (0)
#define DEBUG_LOG(fmt, ...) \
  do { fprintf(stderr, logger, __VA_ARGS__); } while (0)
#else
#define DEBUG_STATE(fmt, ...)
#define DEBUG_REPORT(fmt, ...)
#define DEBUG_LOG(fmt, ...)
#endif

#define NHASH 9997 /* size of symbol table */


/* === LEXER === */
extern int yydebug;         /* debug flag */
extern FILE *yyin;          /* input stream */
extern int yylineno; /* current lexer line */
extern void yyerror(char *s, ...);

extern FILE *logger;

#endif /* DEFINES_H_INCLUDED */
