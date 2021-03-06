%{

#include <stdlib.h>

#include "parser.tab.h"

#include "deimain.h"
#include "value.h"
#include "ast.h"
#include "symbols.h"
#include "eval.h"
#include "result.h"
#include "flags.h"
#include "util.h"

%}

%union {
	struct ast *a;					/* AST nodes */
	struct symbol *s;				/* identifier names */
	struct value *v;				/* value chain building */
	struct stmt_result *r;	/* result state */
	int i;									/* straight number */
	int fn;									/* enum function */
}

%token <i> NUM DNUM PNUM FQUANT SQUANT SSELECT PSELECT
%token <fn> COND F_ADD F_SUB F_MOD CMP GROUP
%token <s> IDENT
%token UNION "||" INTER "&&" DIV "//" RANGE ".." IF "if" XQUANT EXIT EOL

%type <i> nnum fquant ssel
%type <a> group cond math set func die a_args s_args m_args error
%type <r> stmt
%type <v> list

%start line

%nonassoc CMP
%left '+' '-'
%left '*' DIV '%'
%right '^'
%left '&' '|' UNION INTER
%nonassoc UMINUS

	// %destructor { freeAst_Symbol(&($$)); } /* a */  group cond math set func die a_args s_args m_args error
	// %destructor { freeSymbol(&($$)); /* TODO: wipe symbol meaning */ } /* s */  IDENT
	// %destructor { freeValue(&($$)); } /* v */  list
	// %destructor { freeState(&($$)); } /* r */  stmt
	// %destructor {  } /* i */  NUM DNUM PNUM FQUANT SQUANT SSELECT PSELECT nnum fquant ssel
	// %destructor {  } /* fn */ COND F_ADD F_SUB F_MOD CMP GROUP

%%

line:																{  }
	|			'@'													{  }
	|			line stmt EOL								{ 
		gSilent = 0;
		Result *r = NULL;
		switch ($2->type){
			case O_math:
				printAst_Symbol($2->ast);
				r = eval($2->ast);
				printf("= ");
				printResult(r);
				freeAst_Symbol(&($2->ast));
				printf("\n> ");
				break;
			case O_assign:
				printAst_Symbol($2->ast);
				r = eval($2->ast);
				freeAst_Symbol(&($2->ast));
				printf("\n> ");
				break;
			case O_none:
				break;
		}
		freeResult(&r);
		freeState(&($2));
	}
	|			line '@' stmt EOL						{ 
		gSilent = 1;
		Result *r = NULL;
		switch ($3->type){
			case O_math:
				r = eval($3->ast);
				freeAst_Symbol(&($3->ast));
				break;
			case O_assign:
				r = eval($3->ast);
				freeAst_Symbol(&($3->ast));
				break;
			case O_none:
				break;
		}
		freeResult(&r);
		freeState(&($3));
	}
	|			line EXIT EOL								{ YYACCEPT; }
	|			line '@' EXIT EOL						{ YYACCEPT; /* Bison has no duplicate rules */ }
	|			line error EOL							{ printf("!> "); }
	|			line '@' error EOL					{ printf("!"); }
	;

stmt:		group												{ $$ = newState(O_math, $1); }
	|			IDENT ':' cond							{ $$ = newState(O_assign, newAsgn($1, newGroup(D_none, $3, NULL))); }
	|			IDENT '=' cond							{ Result *r = eval($3);
																			$$ = newState(O_assign, newAsgn($1, newGroup(D_none, newNatint(r->integer), NULL)));
																			freeResult(&r);
																			freeAst_Symbol(&($3)); }
	|																	{ $$ = newState(O_none, NULL); }
	;

group:	cond												{ $$ = newGroup(D_none,  $1, NULL); }
	|			cond GROUP									{ $$ = newGroup(		 $2, $1, NULL); }
	|			cond GROUP group						{ $$ = newGroup(		 $2, $1,   $3); }
	;

cond:		math												{ $$ = $1; }
	|			math '?' math ';' cond 			{ $$ = newIfelse($1, $3, $5); }
	|			math '?' math								{ $$ = newIfelse($1, $3, newNatint(0)); }
	;

math:		math '+' math 							{ $$ = newAst('+', $1, $3); }
	|			math '-' math								{ $$ = newAst('-', $1, $3); }
	|			math '*' math								{ $$ = newAst('*', $1, $3); }
	|			math "//" math							{ $$ = newAst(DIV, $1, $3); }
	|			math '%' math								{ $$ = newAst('%', $1, $3); }
	|			math '^' math								{ $$ = newAst('^', $1, $3); }
	|			math CMP math								{ $$ = newCmp($2, $1, $3); }
	|			'-' math			%prec UMINUS	{ $$ = newAst('M', $2, NULL); }
	|			'(' math ')'								{ $$ = $2; }
	|			NUM													{ $$ = newNatint($1); }
	|			IDENT												{ $$ = newSymcall($1); }
	|			set													{ $$ = newAst('S', $1, NULL); }
	;

set:		set '&'  set								{ $$ = newAst( '&', $1, $3); }
	|			set '|'  set								{ $$ = newAst( '|', $1, $3); }
	|			set "&&" set								{ $$ = newAst(INTER, $1, $3); }
	|			set "||" set								{ $$ = newAst(UNION, $1, $3); }
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
	|				PSELECT fquant						{ $$ = newFargs($2, $1,  1, C_none); }
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
	|				SQUANT ssel "if" COND fquant{ $$ = newFargs($5, $2, $1, $4); }
	|				NUM ssel "if" COND fquant		{ $$ = newFargs($5, $2, $1, $4); }
	|				ssel "if" COND fquant				{ $$ = newFargs($4, $1,  1, $3); }
	|				PSELECT fquant 							{ $$ = newFargs($2, $1, -1, C_none); }
	|				PNUM 's' fquant 						{ $$ = newFargs($3, $1, -1, C_none); }
	|				PSELECT "if" COND fquant 		{ $$ = newFargs($4, $1, -1, $3); }
	|				PNUM 's' "if" COND fquant 	{ $$ = newFargs($5, $1, -1, $4); }
	;

ssel: 		SSELECT 									{ $$ = $1; }
	| 			NUM 											{ $$ = $1; }
	;

fquant: 	FQUANT 										{ $$ = $1; }
	| 			NUM XQUANT 								{ $$ = $1; }
	| 																{ $$ =  1; }
	;

die:		DNUM 'd' NUM										{ $$ = newNatdie($1,  1, $3); }
	|			DNUM 'd' '[' nnum ".." nnum']'	{ $$ = newNatdie($1, $4, $6); }
	|			'd' NUM													{ $$ = newNatdie( 1,  1, $2); }
	|			'd' '[' nnum ".." nnum']'				{ $$ = newNatdie( 1, $3, $5); }
	|			DNUM 'd' '{' list '}'						{ $$ = newSetdie($1, $4); }
	|			'd' '{' list '}'								{ $$ = newSetdie( 1, $3); }
	;

list:		nnum												{ $$ = newValue($1, NULL); }
	| 		nnum ',' list								{ $$ = newValue($1, $3); }
	;

nnum:		NUM													{ $$ = $1; }
	|			'-'	NUM											{ $$ = -$2; }
	;

%%

	// line:		line math EOL								{ printAst_Symbol($2); Result *r = eval($2); printf(" = %d\n", r->integer); freeResult(&r); freeAst_Symbol(&$2); printf("\n> "); }
	// 	|			line IDENT ':' math EOL			{ setsym($2, $4); printf("\n> "); }
	// 	|			line error EOL							{ printf("\n!> "); }
	// 	|			line EXIT EOL								{ /* printf("closing!\n"); */ exit(0); }
	// 	|			line EOL										{ printf("> "); }
	// 	|			EOL													{  }
	// 	|			line '@' math EOL						{ Result *r = eval($3); freeResult(&r); freeAst_Symbol(&$3); }
	// 	|			line '@' IDENT ':' math EOL	{ setsym($3, $5); }
	// 	|			line '@' error EOL					{ printf("!") }
	// 	|			line '@' EXIT EOL						{ exit(0); }
	// 	|			line '@' EOL								{  }
	// 	|																	{  }
	// 	;
