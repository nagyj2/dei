%{
#include <stdio.h>
#include <stdlib.h>
#include "dei.tab.h"
#include "dei.h"

int yylex(void);
%}

/* TODO fix union types */
%union { /* types which can be returned by productions */
  struct ast *a;
  struct rollresult *r;
  int d;
  struct symbol *s;   /* which symbol */
  int fn;             /* which function */
}
/* since result is a union, lexer must set yylval.<> accordind
* to the rules (i.e.) if a ast is returned, yylval.a must be set
*/

/* declare tokens */
/* TODO declare tokens and types */
/* %token <d> NUM TIMES
%token <s> IDENT
%token <fn> FUNC CMP */

%token UNION INTER DIV RANGE
%token CMP FUNC SELECT TIMES
%token IDENT NUM

%token EOL

/* TODO fix precidence and types */
%nonassoc CMP
%left '+' '-'
%left '*' DIV '%'
%right '^'
%left UNION INTER
%left '&' '|'
%nonassoc UMINUS

/* set production result types */
/* %type <a> die stmt list */
/* %type <d> calclist */

/* start production */
/* TODO fix start symbol */
%start calcroll

%%

  /* performs math on summed dice rolls */
stmt: stmt CMP stmt         { /*$$ = newcmp($2, $1, $3);*/ }
  |   stmt '+' stmt         { /*$$ = newast('+', $1, $3);*/ }
  |   stmt '-' stmt         { /*$$ = newast('-', $1, $3);*/ }
  |   stmt '*' stmt         { /*$$ = newast('*', $1, $3);*/ }
  |   stmt DIV stmt         { /*$$ = newast(DIV, $1, $3);*/ }
  |   stmt '%' stmt         { /*$$ = newast('%', $1, $3);*/ }
  |   stmt '^' stmt         { /*$$ = newast('^', $1, $3);*/ }
  |   '-' stmt %prec UMINUS { /*$$ = newast('M', $2, NULL);*/ }
  |   set                   {  }
  ;

  /* performs math on not summed rolls */
set:  set '&' set           { /*$$ = newast('&', $1, $3);*/ }
  |   set '|' set           { /*$$ = newast('|', $1, $3);*/ }
  |   set INTER set         { /*$$ = newast(INTER, $1, $3);*/ }
  |   set UNION set         { /*$$ = newast(UNION, $1, $3);*/ }
  |   '(' set ')'           { /*$$ = $2;*/ }
  |   math                  {  }
  ;

  /* a single die roll result */
math: die mod             {  }
  ;

  /* modifiers which modify the die rolls themselves */
mod:  FUNC SELECT times_n mod   {  }
  |   FUNC math times_n mod     {  }
  |                             {  }
  ;

  /* how many times the die roll modifer should occur */
times_n:  TIMES   {  }
  |
  ;

  /* value literals - both die, number and ident */
  /* return die roll (ints are a die roll with a value equaling the int) */
die:  NUM 'd' NUM                 {  }
  |   NUM 'd' '{' stmt list '}'       {  }
  |   NUM 'd' '{' stmt RANGE stmt '}' {  }
  |   'd' NUM                     {  }
  |   'd' '{' stmt list '}'       {  }
  |   'd' '{' stmt RANGE stmt '}' {  }
  |   '[' stmt list ']'         {  }
  |   NUM                       { /*$$ = newnum($1);*/ }
  |   IDENT                     { /*$$ = newref($1);*/ }
  ;

  /* list of integers - used to determine die faces or hardcoded results */
list: ',' stmt list  { /*
      if ($3 == NULL) $$ = $1;
      else $$ = newast('L', $1, $3); */ /* create list of statements */
  }
  |                 {  }
  ;

  /* top level production. defines a valid sentance */
calcroll: calcroll stmt EOL   {
            printf("parsed!\n> ");
            /*printf("= %d\n> ", eval($2));*/
            /*treefree($2);*/
  }
  |       calcroll IDENT ':' stmt EOL {
            printf("defined!\n> ");
            /*dodef($2, $4);
            printf("Defined %s\n> ", $2->name);*/
  }
  |       calcroll error EOL  {
            /* error is a special token produced in event of error */
            yyerrok;
            printf("error!\n> ");
  }
  |       calcroll EOL        {  }
  |   { /* nothing */ }
  ;

%%
