%{
#include <stdlib.h>
#include <stdio.h> /* needed for FILE */
#include <stdarg.h> /* needed for yyerror */

#include "dei.tab.h"

extern int yydebug;         /* debug flag */
extern FILE *yyin;          /* input stream */
extern int yylineno; /* current lexer line */
extern void yyerror(char *s, ...);
extern int yyparse(void);
extern int yylex(void);
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

int main(int argc, char **argv){

  if (argc > 1){
    if (!(yyin = fopen(argv[1], "r"))){
      perror(argv[1]);
      return 1;
    }
  }else{
    printf("> ");
  }

  return yyparse();
}

void yyerror(char *s, ...){
  va_list ap;
  va_start(ap, s);

  fprintf(stderr, "%d: error : ", yylineno);
  vfprintf(stderr, s, ap);
  fprintf(stderr, "\n");
}
