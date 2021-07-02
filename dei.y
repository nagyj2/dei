%{
#include <stdio.h>
#include <stdlib.h>
#include "dei.tab.h"
#include "dei.h"

int yylex(void);
%}

%union { /* types which can be returned by productions */
  struct ast *a;
  int d;
  struct symbol *s;   /* which symbol */
  struct symlist *sl;
  int fn;             /* which function */
}
/* since result is a union, lexer must set yylval.<> accordind
* to the rules (i.e.) if a ast is returned, yylval.a must be set
*/

/* declare tokens */
%token <d> NUM
%token <s> IDENT
%token <fn> FUNC
%token EOL
%token IF THEN ELSE WHILE DO LET

%nonassoc <fn> CMP
%right '='
%left '+' '-'
%left '*' DIV '%'
%nonassoc UMINUS

/* set production result types */
%type <a> exp stmt list explist
%type <sl> symlist
/* %type <d> calclist */

/* start production */
%start calclist

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

%%

stmt: IF exp THEN list            { $$ = newflow('I', $2, $4, NULL); }
  |   IF exp THEN list ELSE list  { $$ = newflow('I', $2, $4, $6); }
  |   WHILE exp DO list           { $$ = newflow('W', $2, $4, NULL); }
  |   exp                         { $$ = $1; }
  ;

list: /* nothing */ { $$ = NULL; }
  | stmt ';' list   {
      if ($3 == NULL) $$ = $1;
      else $$ = newast('L', $1, $3); /* create list of statements */
  }
  | stmt            { $$ = $1 }
  ;

exp:  exp CMP exp           { $$ = newcmp($2, $1, $3); }
  |   exp '+' exp           { $$ = newast('+', $1, $3); }
  |   exp '-' exp           { $$ = newast('-', $1, $3); }
  |   exp '*' exp           { $$ = newast('*', $1, $3); }
  |   exp DIV exp           { $$ = newast(DIV, $1, $3); }
  |   exp '%' exp           { $$ = newast('%', $1, $3); }
  |   '(' exp ')'           { $$ = $2; }
  |   '-' exp %prec UMINUS  { $$ = newast('M', $2, NULL); }
  |   NUM                   { $$ = newnum($1); }
  |   IDENT                 { $$ = newref($1); }
  |   IDENT '=' exp         { $$ = newasgn($1, $3); }
  |   FUNC '(' explist ')'  { $$ = newfunc($1, $3); }
  |   IDENT '(' explist ')' { $$ = newcall($1, $3); }
  ;

explist:  exp         { $$ = $1; }
  | exp ',' explist   { $$ = newast('L', $1, $3); }
  ;

symlist:  IDENT        { $$ = newsymlist($1, NULL); }
  | IDENT ',' symlist  { $$ = newsymlist($1, $3); }
  ;

calclist: /* nothing */
  | calclist stmt EOL {
    printf("= %d\n> ", eval($2));
    treefree($2);
  }
  | calclist LET IDENT '(' symlist ')' '=' list EOL {
    dodef($3, $5, $8);
    printf("Defined %s\n> ", $3->name);
  }
  | calclist LET IDENT '=' list EOL {
    dosym($3, $5);
    printf("Defined %s\n> ", $3->name);
  }
  | calclist error EOL {
    /* error is a special token produced in event of error */
    yyerrok;
    printf("> ");
  }
  ;


%%
