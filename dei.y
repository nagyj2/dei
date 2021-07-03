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
/* %type <a> exp stmt list explist */
/* %type <d> calclist */

/* start production */
/* TODO fix start symbol */
%start calcroll


%%

stmt: exp                   {  }
  |   exp mod               {  }
  ;

mod:  FUNC select times_n mod   {  }
  |   FUNC math times_n mod     {  }
  |                             {  }
  ;

select: SELECT    {  }
  |     math      {  }
  ;

times_n:  TIMES   {  }
  |
  ;

exp:  exp CMP exp           { /*$$ = newcmp($2, $1, $3);*/ }
  |   exp '+' exp           { /*$$ = newast('+', $1, $3);*/ }
  |   exp '-' exp           { /*$$ = newast('-', $1, $3);*/ }
  |   exp '*' exp           { /*$$ = newast('*', $1, $3);*/ }
  |   exp DIV exp           { /*$$ = newast(DIV, $1, $3);*/ }
  |   exp '%' exp           { /*$$ = newast('%', $1, $3);*/ }
  |   exp '^' exp           { /*$$ = newast('^', $1, $3);*/ }
  |   '-' exp %prec UMINUS  { /*$$ = newast('M', $2, NULL);*/ }
  |   set                   {  }
  ;

set:  set '&' set           { /*$$ = newast('&', $1, $3);*/ }
  |   set '|' set           { /*$$ = newast('|', $1, $3);*/ }
  |   set INTER set         { /*$$ = newast(INTER, $1, $3);*/ }
  |   set UNION set         { /*$$ = newast(UNION, $1, $3);*/ }
  |   '(' set ')'           { /*$$ = $2;*/ }
  |   math                  {  }
  ;

math: NUM 'd' NUM               {  }
  |   'd' NUM                   {  }
  |   'd' '{' exp list '}'      {  }
  |   'd' '{' exp RANGE exp '}' {  }
  |   '[' list ']'              {  }
  |   NUM                       { /*$$ = newnum($1);*/ }
  |   IDENT                     { /*$$ = newref($1);*/ }
  ;


list: ',' exp list  { /*
      if ($3 == NULL) $$ = $1;
      else $$ = newast('L', $1, $3); */ /* create list of statements */
  }
  |                 {  }
  ;

calcroll: calcroll stmt EOL {
            printf("parsed!\n> ");
            /*printf("= %d\n> ", eval($2));*/
            /*treefree($2);*/
  }
  |       calcroll IDENT ':' stmt EOL {
            printf("defined!\n> ");
            /*dodef($2, $4);
            printf("Defined %s\n> ", $2->name);*/
  }
  |       calcroll error EOL {
            /* error is a special token produced in event of error */
            yyerrok;
            printf("error!\n> ");
  }
  |   { /* nothing */ }
  ;

%%
