
%{

#include <stdlib.h>

#include "parser.tab.h"

#include "deimain.h"
#include "struct.h"
#include "ast.h"
#include "eval.h"

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
%type <a> math set func die a_args s_args m_args
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
	|				SSELECT fquant						{ $$ = newFargs($2, $1,  1, C_none); }
	|				NUM fquant								{ $$ = newFargs($2, $1,  1, C_none); }
	|				PSELECT fquant						{ $$ = newFargs($2, $1, -1, C_none); }
	;

s_args:		SQUANT ssel								{ $$ = newFargs( 1, $2, $1, C_none); }
	|				NUM ssel									{ $$ = newFargs( 1, $2, $1, C_none); }
	|				NUM												{ $$ = newFargs( 1, $1,  1, C_none); }
	|				SSELECT										{ $$ = newFargs( 1, $1,  1, C_none); }
	|				PSELECT										{ $$ = newFargs( 1, $1, -1, C_none); }
	|				PNUM 's'									{ $$ = newFargs( 1, $1, -1, C_none); }
	;

m_args:		SQUANT ssel fquant 					{ $$ = newFargs($3, $2, $1, C_none); }
	|				NUM ssel fquant 						{ $$ = newFargs($3, $2, $1, C_none); }
	|				ssel fquant 								{ $$ = newFargs($2, $1,  1, C_none); }
	|				SQUANT ssel IF COND fquant	{ $$ = newFargs($5, $2, $1, $4); }
	|				NUM ssel IF COND fquant			{ $$ = newFargs($5, $2, $1, $4); }
	|				ssel IF COND fquant					{ $$ = newFargs($4, $1,  1, $3); }
	|				PSELECT fquant 							{ $$ = newFargs($2, $1, -1, C_none); }
	|				PNUM 's' fquant 						{ $$ = newFargs($3, $1, -1, C_none); }
	|				PSELECT IF COND fquant 			{ $$ = newFargs($4, $1, -1, $3); }
	|				PNUM 's' IF COND fquant 		{ $$ = newFargs($5, $1, -1, $4); }
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

line:		line math EOL								{ printAst($2); struct result *r = eval($2); printf(" = %d\n", r->integer); freeResult(&r); freeAst(&$2); printf("\nparsed!\n> "); }
	|			line IDENT ':' math EOL			{ setsym($2, $4); printf("saved!\n> "); }
	|			line error EOL							{ printf("error!\n> "); }
	|			line EXIT EOL								{ printf("closing!\n"); exit(0); }
	|			line EOL										{ printf("> "); }
	|			EOL													{  }
	|			line '@' math EOL						{ struct result *r = eval($3); freeResult(&r); freeAst(&$3); }
	|			line '@' IDENT ':' math EOL	{ setsym($3, $5); }
	|			line '@' error EOL					{ printf("silent error!\n") }
	|			line '@' EXIT EOL						{ exit(0); }
	|			line '@' EOL								{  }
	|																	{  }
	;

%%
