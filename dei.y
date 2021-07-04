%{
#include <stdio.h>
#include <stdlib.h>

#include "dei.tab.h"
#include "dei.h"

int yylex(void);
%}

/* TODO fix union types */
/* since result is a union, lexer must set yylval.<> accordind
 * to the rules (i.e.) if a ast is returned, yylval.a must be set
 */
%union { /* types which can be returned by productions */
  struct ast *a;      /* parseable ast */
  struct symbol *s;   /* which symbol */
  struct value *v;    /* chain of int values */
  int fn;             /* which function / option */
  int d;              /* raw integer */
}

/* declare tokens */
/* TODO declare tokens and types */
%token <d> NUM DNUM
%token <s> IDENT
%token <fn> FUNC SELECT CMP
%token UNION INTER DIV RANGE
%token QUANT XQUANT
%token EOL

%type <a> math dice func die
%type <v> list

%nonassoc CMP     /* nonassoc will disallow sequential OPs */
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
%start start

%%

  /* performs math on numbers */
math: math CMP math											{ $$ = newcmp($2, $1, $3); }
  |   math '+' math											{ $$ = newast('+', $1, $3); }
  |   math '-' math											{ $$ = newast('-', $1, $3); }
  |   math '*' math											{ $$ = newast('*', $1, $3); }
  |   math DIV math											{ $$ = newast(DIV, $1, $3); }
  |   math '%' math											{ $$ = newast('%', $1, $3); }
  |   math '^' math											{ $$ = newast('^', $1, $3); }
  |   '-' math 				%prec UMINUS			{ $$ = newast('M', $2, NULL); }
  |   NUM                               { $$ = newint($1); }
  |   dice															{ $$ = newsum($1); }
  ;

  /* performs math on multiple die's rolls */
dice: dice '&' dice											{ $$ = $1; }
  |   dice '|' dice											{ $$ = $1; }
  |   dice INTER dice										{ $$ = $1; }
  |   dice UNION dice										{ $$ = $1; }
  |   '(' math ')'											{ $$ = $2; }
  |   func															{ $$ = $1; }
  ;

  /* performs math on a single die rolls */
func: die																{ $$ = $1; }
  |		func FUNC SELECT									{ $$ = $1; }
  |   func FUNC SELECT QUANT						{ $$ = $1; }
  |   func FUNC SELECT NUM XQUANT	  		{ $$ = $1; }
  |   func FUNC NUM									    { $$ = $1; }
  |   func FUNC NUM QUANT							  { $$ = $1; }
  |   func FUNC NUM NUM XQUANT				  { $$ = $1; }
	;

  /* performs a die roll */
die:  DNUM 'd' NUM											{ $$ = newroll($1, newdie(1, $3)); }
  |   DNUM 'd' '{' list '}'							{ $$ = newroll($1, setdie($4)); }
  |   DNUM 'd' '{' NUM RANGE NUM '}'    { $$ = newroll($1, newdie($4, $6)); }
  |   'd' NUM														{ $$ = newroll(1, newdie(1, $2)); }
  |   'd' '{' list '}'									{ $$ = newroll(1, setdie($3)); }
  |   'd' '{' NUM RANGE NUM '}'				  { $$ = newroll(1, newdie($3, $5)); }
  |   '[' list ']'											{ $$ = setroll(setdie($2)); }
  |   IDENT															{ $$ = newref($1); }
  ;

  /* creates a list of values */
list: NUM                               { $$ = addvalue($1, NULL); }
  |		NUM ',' list                      { $$ = addvalue($1, $3); }
  ;

  /* performs top-level actions */
start:start math EOL										{ printf("= %d\n> ", eval($2)->ivalue); /*treefree($2);*/ }
  |   start IDENT ':' math EOL					{ printf("saved!\n> "); }
  |   start error EOL										{ printf("error!\n> "); }
  |   start EOL													{ printf("> "); }
  |
  ;

%%

/* die: '['  ']' {  } */
