
#include <stdarg.h> /* needed for yyerror */

#include "parser.tab.h"

#include "deimain.h"

int main(int argc, char **argv){
	char *stdsave = "config.dei";

	FILE *f = fopen(stdsave, "r");
	if (f){
		printf("found save!\n");
		yyin = f;
		yyparse();
		fclose(yyin);
		printf("\n");
	}

	int i;
	for(i = 1; i < argc; i++) {
		f = fopen(argv[i], "r");

		if(!f) {
			perror(argv[i]);
			return (1);
		}

		printf("new file!\n");
		yyin = f;
		yylineno = 1;
		yyrestart(yyin);
		yyparse();
		fclose(yyin);
		printf("\n");
  }

	printf("stdin!\n> ");
	yyin = stdin;
	yylineno = 1;
	yyrestart(yyin);
  return yyparse();
}

void yyerror(char *s, ...){
  va_list ap;
  va_start(ap, s);

  fprintf(stderr, "%d: error : ", yylineno);
  vfprintf(stderr, s, ap);
  fprintf(stderr, "\n");
}