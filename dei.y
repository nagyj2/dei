%{
#include <stdio.h>
#include <stdlib.h>
#include "dei.tab.h"
#include "dei.h"

int yylex(void);
/* void yyerror(char *s); */
%}

/* declare tokens */
%token NUM
%token PLUS MINUS MULT DIV MOD EXPN
%token SETINTER SETUNION ELEMINTER ELEMUNION
%token EQ NE LT GT LE GE
%token LPAREN RPAREN
%token LBRACE RBRACE
%token COMMA
%token RANGE
%token DIEFACE
%token DROP
%token APPEND
%token CHOOSE
%token REROLL
%token COUNT
%token HIGH
%token LOW
%token RAND
%token SET
%token ALL
%token TIME
%token EOL

%start calc /* start production */

%union { /* types which can be returned by productions */
  struct ast *a;
  int d;
}
/* since result is a union, lexer must set yylval.<> accordind
 * to the rules (i.e.) if a ast is returned, yylval.a must be set
 */

%type <d> NUM TIME /* set production result types */
%type <a> exp factor term

%%

calc: /* match input beginning and do nothing */
  | calc exp EOL {
      printf("= %d\n", eval($2));
      treefree($2);
      printf("> ");
  }
  | calc EOL {
      printf("> ");
  }
  ;

exp:  factor            /* default: $$ = $1 */
  |   exp '+' factor  { $$ = newast('+', $1, $3); }
  |   exp '-' factor  { $$ = newast('-', $1, $3); }
  ;

factor: term            /* default: $$ = $1 */
  |   factor '*' term   { $$ = newast('*', $1, $3); }
  |   factor DIV term   { $$ = newast('/', $1, $3); }
  |   factor '%' term   { $$ = newast('%', $1, $3); }
  ;

term: NUM { $$ = newnum($1); }
  |   '(' exp ')' { $$ = $2; }
  |   '-' term { $$ = newast('M', $2, NULL); }
  ;

%%

/* int main(int argc, char **argv){
  yyparse();
} */

/* void yyerror(char *s){
  fprintf(stderr, "error: %s\n", s);
} */
