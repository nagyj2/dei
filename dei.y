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
  struct ast *a;
  struct rollresult *r;
  struct rollset *rs;
  struct symbol *s;   /* which symbol */
  int fn;             /* which function / option */
  int d;              /* raw integer */
}

/* declare tokens */
/* TODO declare tokens and types */
%token /*<d>*/ QUANT NUM DNUM
%token /*<s>*/ IDENT
%token /*<fn>*/ FUNC SELECT CMP

%token UNION INTER DIV RANGE
%token EOL

/*%type <a> stmt set math mod
%type <r> die
%type <rs> list
%type <d> times_n/*


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
%start start

%%

  /* performs math on numbers */
math: math CMP math											{  }
  |   math '+' math											{  }
  |   math '-' math											{  }
  |   math '*' math											{  }
  |   math DIV math											{  }
  |   math '%' math											{  }
  |   math '^' math											{  }
  |   '-' math 				%prec UMINUS			{  }
  |   NUM                               {  }
  |   dice															{  }
  ;

  /* performs math on multiple die's rolls */
dice: dice '&' dice											{  }
  |   dice '|' dice											{  }
  |   dice INTER dice										{  }
  |   dice UNION dice										{  }
  |   '(' math ')'											{  }
  |   func															{  }
  ;

  /* performs math on a single die rolls */
func: die																{  }
  |		func FUNC SELECT									{  }
  |   func FUNC SELECT QUANT						{  }
  |   func FUNC NUM									    {  }
  |   func FUNC NUM QUANT							  {  }
	;

  /* performs a die roll */
die:  DNUM 'd' NUM											{  }
  |   DNUM 'd' '{' list '}'							{  }
  |   DNUM 'd' '{' math RANGE math '}'  {  }
  |   'd' NUM														{  }
  |   'd' '{' list '}'									{  }
  |   'd' '{' math RANGE math '}'				{  }
  |   '[' list ']'											{  }
  |   '['  ']'											    {  }
  |   IDENT															{  }
  ;

  /* creates a list of values */
list: math															{  }
  |		math ',' list											{  }
  ;

  /* performs top-level actions */
start:start math EOL										{ printf("parsed!\n> "); }
  |   start IDENT ':' math EOL					{ printf("saved!\n> "); }
  |   start error EOL										{ printf("error!\n> "); }
  |   start EOL													{ printf("> "); }
  |
  ;

%%
