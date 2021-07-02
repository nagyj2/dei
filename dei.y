%{
#include <stdio.h>
#include <stdlib.h>
#include "dei.tab.h"
#include "dei.h"

int yylex(void);
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

/* start production */
%start calc

%union { /* types which can be returned by productions */
  struct ast *a;
  int d;
}
/* since result is a union, lexer must set yylval.<> accordind
 * to the rules (i.e.) if a ast is returned, yylval.a must be set
 */

%left '+' '-'
%left '*' DIV '%'
%nonassoc UMINUS

/* set production result types */
/* %type <d> NUM  */
%type <a> exp calc
%type <d> NUM

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

exp :  exp '+' exp { $$ = newast('+', $1, $3); }
    |  exp '-' exp { $$ = newast('-', $1, $3); }
    |  exp '*' exp { $$ = newast('*', $1, $3); }
    |  exp DIV exp { $$ = newast(DIV, $1, $3); }
    |  exp '%' exp { $$ = newast('%', $1, $3); }
    |  '(' exp ')' { $$ = $2; }
    |  '-' exp %prec UMINUS { $$ = newast('M', $2, NULL); }
    |  NUM         { $$ = newnum($1); }
    ;

%%
