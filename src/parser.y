
%{

#include <stdlib.h>

#include "dei.tab.h"

#include "deimain.h"

%}

%token /*int*/ NUM DNUM FQUANT SQUANT
%token /*fn*/ COND SSELECT PSELECT F_ADD F_SUB F_MOD CMP
%token /*str*/ IDENT
%token UNION INTER DIV RANGE IF XQUANT EXIT EOL

%start stmt

%nonassoc CMP
%left '+' '-'
%left '*' DIV '%'
%right '^'
%left '&' '|' UNION INTER
%nonassoc UMINUS

%%

math:		math '+' math 							{}
	|			math '-' math								{}
	|			math '*' math								{}
	|			math DIV math								{}
	|			math '%' math								{}
	|			math '^' math								{}
	|			math CMP math								{}
	|			'-' math			%prec UMINUS	{}
	|			'(' math ')'								{}
	|			NUM													{}
	|			IDENT												{}
	|			set													{}
	;

set:		set '&' set									{}
	|			set '|' set									{}
	|			set INTER set								{}
	|			set UNION set								{}
	|			func												{}
	;

func:		ndie												{}
	|			sdie												{}
	|			'{' list '}'								{}
	|			func F_ADD selector fquant	{}
	|			func F_SUB selector					{}
	|			func F_MOD selector fquant	{}
	|			func F_MOD selector IF COND fquant	{}
	;

selector:	NUM SSELECT								{}
	|				NUM NUM										{}
	|				SQUANT SSELECT						{}
	|				SQUANT NUM								{}
	|				SSELECT										{} /* SQUANT = 1 for this and below */
	|				NUM												{}
	|				PSELECT										{}
	|				NUM 's'										{}
	;

fquant:		NUM	XQUANT								{}
	|				FQUANT										{}
	|				      										{}
	;

ndie:		DNUM 'd' NUM								{}
	|			DNUM 'd' '[' nnum RANGE nnum']'	{}
	|			'd' NUM											{}
	|			'd' '[' nnum RANGE nnum']'	{}
	;

sdie:		DNUM 'd' '{' list '}'				{}
	|			'd' '{' list '}'						{}
	;

list:		nnum												{}
	| 		nnum ',' list								{}
	;

nnum:		NUM													{}
	|			'-'	NUM											{}
	;

stmt:		stmt math EOL								{ printf("parsed!\n> "); }
	|			stmt IDENT ':' math EOL			{ printf("saved!\n> "); }
	|			stmt error EOL							{ printf("error!\n> "); }
	|			stmt EXIT EOL								{ printf("closing!\n"); exit(0); }
	|			stmt EOL										{ printf("> "); }
	|			stmt '@' math EOL						{ /*printf(".");*/ }
	|			stmt '@' IDENT ':' math EOL	{ /*printf(":");*/ }
	|			stmt '@' error EOL					{ printf("silent error!\n") }
	|			stmt '@' EXIT EOL						{ exit(0); }
	|			stmt '@' EOL								{  }
	|																	{  }
	;

%%
