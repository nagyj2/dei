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
%token <d> NUM DNUM QUANT
%token <s> IDENT
%token <fn> FUNC SELECT CMP
%token UNION INTER DIV RANGE
%token XQUANT
%token EXIT EOL

/* set production result types */
%type <a> math dice func ndie sdie
%type <v> list

%start start

%nonassoc CMP     /* nonassoc will disallow sequential OPs */
%left '+' '-'
%left '*' DIV '%'
%right '^'
%left UNION INTER
%left '&' '|'
%nonassoc UMINUS
%nonassoc PAREN

%%

  /* performs math on numbers */
  /* REPRESENT: integers */
math: math CMP math											{ $$ = newcmp($2, $1, $3); }
  |   math '+' math											{ $$ = newast('+', $1, $3); }
  |   math '-' math											{ $$ = newast('-', $1, $3); }
  |   math '*' math											{ $$ = newast('*', $1, $3); }
  |   math DIV math											{ $$ = newast(DIV, $1, $3); }
  |   math '%' math											{ $$ = newast('%', $1, $3); }
  |   math '^' math											{ $$ = newast('^', $1, $3); }
  |   '-' math 				%prec UMINUS			{ $$ = newast('M', $2, NULL); }
  |   '(' math ')'		%prec PAREN			  { $$ = $2; }
  |   NUM                               { $$ = newnatint($1); }
  |   IDENT															{ $$ = newref($1); }
  |   dice															{ $$ = newast('S', $1, NULL); }
  ;

  /* performs math on multiple die's rolls */
    /* REPRESENT: set of rolls */
dice: dice '&' dice											{ $$ = newast('&', $1, $3); }
  |   dice '|' dice											{ $$ = newast('|', $1, $3); }
  |   dice INTER dice										{ $$ = newast(INTER, $1, $3); }
  |   dice UNION dice										{ $$ = newast(UNION, $1, $3); }
  |   '{' dice '}'											{ $$ = $2; }
  |   func															{ $$ = $1; }
  ;

  /* performs math on a single die rolls */
  /* REPRESENT: set of rolls -> if IDENT is a number, convert to a 1 time die roll */
func: ndie															{ $$ = newast('R', $1, NULL); }
  |   sdie                              { $$ = newast('r', $1, NULL); }
  |   '[' list ']'											{ $$ = newsetres($2); }
  |		func FUNC SELECT									{ $$ = newfunc($2, $3, 1, $1); }
  |   func FUNC SELECT QUANT						{ $$ = newfunc($2, $3, $4, $1); }
  |   func FUNC SELECT NUM XQUANT	  		{ $$ = newfunc($2, $3, $4, $1); }
  |   func FUNC NUM									    { $$ = newfunc($2, $3, 1, $1); }
  |   func FUNC NUM QUANT							  { $$ = newfunc($2, $3, $4, $1); }
  |   func FUNC NUM NUM XQUANT				  { $$ = newfunc($2, $3, $4, $1); }
  ;

  /* performs a die roll */
  /* REPRESENT: dice -> faces, no. of rolls */
ndie:  DNUM 'd' NUM											{ $$ = newnatdie($1, 1, $3); }
  |   DNUM 'd' '{' NUM RANGE NUM '}'    { $$ = newnatdie($1, $4, $6); }
  |   'd' NUM														{ $$ = newnatdie(1, 1, $2); }
  |   'd' '{' NUM RANGE NUM '}'				  { $$ = newnatdie(1, $3, $5); }
  ;

sdie: DNUM 'd' '{' list '}'							{ $$ = newsetdie($1, $4); }
  |   'd' '{' list '}'									{ $$ = newsetdie(1, $3); }

  /* creates a list of values */
list: NUM                               { $$ = newvalue($1, NULL); }
  |		NUM ',' list                      { $$ = newvalue($1, $3); }
  ;

  /* performs top-level actions */
start:start math EOL										{
        struct result *r = eval($2);
        /* printf("out <- "); */ printtree($2);
        printf(" = %d",r->i);
        resultfree(r);
        treefree($2);
        printf("\n> ");
  }
  |   start IDENT ':' math EOL					{
        printtree($4);
        setsym($2,$4);
        //treefree($4); /* CANNOT FREE */
        printf("\n> ");
  }
  |   start error EOL										{ printf("error!\n> "); }
  |   start EOL													{ printf("> "); }
  |   start EXIT EOL						        { exit(0); }
  |
  ;

%%

/* die: '['  ']' {  } */
