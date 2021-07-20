
%{

#include <stdlib.h>

#include "parser.tab.h"

#include "deimain.h"
#include "struct.h"
#include "ast.h"

%}

%union {
	struct ast *a;		/* AST nodes */
	struct symbol *s;	/* identifier names */
	struct value *v;	/* value chain building */
	int i;						/* straight number */
	int fn;						/* enum function */
}

%token <i> NUM DNUM PNUM FQUANT SQUANT SSELECT PSELECT
%token <fn> COND F_ADD F_SUB F_MOD CMP
%token <s> IDENT
%token UNION INTER DIV RANGE IF XQUANT EXIT EOL

%type <i> nnum fquant ssel
%type <a> stmt math set func die a_args s_args m_args
%type <v> list

%start line

%nonassoc CMP
%left '+' '-'
%left '*' DIV '%'
%right '^'
%left '&' '|' UNION INTER
%nonassoc UMINUS

%%

math:		math '+' math 							{ $$ = newAst('+', $1, $3); }
	|			math '-' math								{ $$ = newAst('-', $1, $3); }
	|			math '*' math								{ $$ = newAst('*', $1, $3); }
	|			math DIV math								{ $$ = newAst(DIV, $1, $3); }
	|			math '%' math								{ $$ = newAst('%', $1, $3); }
	|			math '^' math								{ $$ = newAst('^', $1, $3); }
	|			math CMP math								{ $$ = newCmp($2, $1, $3); }
	|			'-' math			%prec UMINUS	{ $$ = newAst('M', $2, NULL); }
	|			'(' math ')'								{ $$ = $2; }
	|			NUM													{ $$ = newNatint($1); }
	|			IDENT												{ $$ = newSymcall($1); }
	|			set													{ $$ = newAst('S', $1, NULL); }
	;

set:		set '&' set									{ $$ = newAst('&', $1, $3); }
	|			set '|' set									{ $$ = newAst('|', $1, $3); }
	|			set INTER set								{ $$ = newAst(INTER, $1, $3); }
	|			set UNION set								{ $$ = newAst(UNION, $1, $3); }
	|			'[' set ']'									{ $$ = $2 }
	|			func												{ $$ = newAst('Z', $1, NULL); }
	;

func:		die													{ $$ = newAst('R', $1, NULL); }
	|			'{' list '}'								{ $$ = newSetres($2); }
	|			func F_ADD a_args						{ $$ = newFunc($2, $1, $3); }
	|			func F_SUB s_args						{ $$ = newFunc($2, $1, $3); }
	|			func F_MOD m_args						{ $$ = newFunc($2, $1, $3); }
	;

a_args:		SQUANT ssel fquant				{ $$ = newFargs($3, $2, $1, C_none); }
	|				NUM ssel fquant						{ $$ = newFargs($3, $2, $1, C_none); }
	|				ssel fquant								{ $$ = newFargs($2, $1,  1, C_none); }
	|				PSELECT fquant						{ $$ = newFargs($2, $1, S_all, C_none); }
	|				PNUM 's' fquant						{ $$ = newFargs($3, $1, S_all, C_none); }
	;

s_args:		SQUANT ssel								{ $$ = newFargs( 1, $2, $1, C_none); }
	|				NUM ssel									{ $$ = newFargs( 1, $2, $1, C_none); }
	|				ssel											{ $$ = newFargs( 1, $1,  1, C_none); }
	|				PSELECT										{ $$ = newFargs( 1, $1, S_all, C_none); }
	|				PNUM 's'									{ $$ = newFargs( 1, $1, S_all, C_none); }
	;

m_args:		SQUANT ssel fquant 					{ $$ = newFargs($3, $2, $1, C_none); }
	|				NUM ssel fquant 						{ $$ = newFargs($3, $2, $1, C_none); }
	|				ssel fquant 								{ $$ = newFargs($2, $1,  1, C_none); }
	|				SQUANT ssel IF COND fquant	{ $$ = newFargs($5, $2, $1, $4); }
	|				NUM ssel IF COND fquant			{ $$ = newFargs($5, $2, $1, $4); }
	|				ssel IF COND fquant					{ $$ = newFargs($4, $1,  1, $3); }
	|				PSELECT fquant 							{ $$ = newFargs($2, $1, S_all, C_none); }
	|				PNUM 's' fquant 						{ $$ = newFargs($3, $1, S_all, C_none); }
	|				PSELECT IF COND fquant 			{ $$ = newFargs($4, $1, S_all, $3); }
	|				PNUM 's' IF COND fquant 		{ $$ = newFargs($5, $1, S_all, $4); }
	;

ssel: 		SSELECT 									{ $$ = $1; }
	| 			NUM 											{ $$ = $1; }
	;

fquant: 	FQUANT 										{ $$ = $1; }
	| 			NUM XQUANT 								{ $$ = $1; }
	| 																{ $$ =  1; }
	;

die:		DNUM 'd' NUM								{ $$ = newNatdie($1,  1, $3); }
	|			DNUM 'd' '[' nnum RANGE nnum']'	{ $$ = newNatdie($1, $4, $6); }
	|			'd' NUM											{ $$ = newNatdie( 1,  1, $2); }
	|			'd' '[' nnum RANGE nnum']'	{ $$ = newNatdie( 1, $3, $5); }
	|			DNUM 'd' '{' list '}'				{ $$ = newSetdie($1, $4); }
	|			'd' '{' list '}'						{ $$ = newSetdie( 1, $3); }
	;

list:		nnum												{ $$ = newValue($1, NULL); }
	| 		nnum ',' list								{ $$ = newValue($1, $3); }
	;

nnum:		NUM													{ $$ = $1; }
	|			'-'	NUM											{ $$ = -$2; }
	;

stmt:		math 												{ $$ = $1; }
	| 		IDENT ':' math							{ $$ = newAsgn($1, $3); }
	;

line: 	line stmt EOL 							{ printAst($2); freeAst(&$2); printf("\nparsed!\n> "); }
	|			line '@' stmt EOL						{ freeAst(&$3); }
	|			line error EOL							{ printf("error!\n> "); }
	|			line '@' error EOL					{  }
	|			line EOL										{  }
	| 		line EXIT EOL								{ printf("closing!\n"); exit(0); }
	| 		line '@' EXIT EOL						{ exit(0); }
	|			line '@' EOL								{  }
	|																	{  }
	;

%%

/*
line:		line math EOL								{ printAst($2); freeAst(&$2); printf("\nparsed!\n> "); }
	|			line IDENT ':' math EOL			{ printf("saved!\n> "); }
	|			line error EOL							{ printf("error!\n> "); }
	|			line EXIT EOL								{ printf("closing!\n"); exit(0); }
	|			line EOL										{ printf("> "); }
	|			line '@' math EOL						{  }
	|			line '@' IDENT ':' math EOL	{  }
	|			line '@' error EOL					{ printf("silent error!\n") }
	|			line '@' EXIT EOL						{ exit(0); }
	|			line '@' EOL								{  }
	|																	{  }
	;

psel: 		PSELECT 									{ $$ = $1; }
	| 			NUM 's' 									{ $$ = $1; }
	;

squant: 	SQUANT 										{ $$ = $1; }
	| 			NUM 											{ $$ = $1; }
	| 																{ $$ =  1; }
	;

*/
