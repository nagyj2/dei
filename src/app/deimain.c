/** Implementation for the entry points of Dei.
 * @file deimain.c
 * @author Jason Nagy (jaysun_n@hotmail.com)
 * @version 0.1
 * @date 2021-07-26
 * 
 * @copyright Copyright (c) 2021
 * 
 */

#include <stdlib.h> /* srand */
#include <stdarg.h> /* va_list */
#include <time.h> /* time */

#include "deimain.h"
#include "value.h"
#include "ast.h"
#include "symbols.h"
#include "result.h"
#include "eval.h"


int main(int argc, char **argv) {

	/* seed random number generator */
	srand(time(NULL));

	#ifdef TESTS
	test_value();
	return 0;
	#endif

	#ifdef DEBUG
	printf("ValueChain size: %lu\n", sizeof(ValueChain));
	printf("Symbol size: %lu\n", sizeof(Symbol));
	printf("AST size: %lu\n", sizeof(AST));
	printf("NatDie size: %lu\n", sizeof(NatDie));
	printf("SetDie size: %lu\n", sizeof(SetDie));
	printf("NatInt size: %lu\n", sizeof(NatInt));
	printf("FuncArgs size: %lu\n", sizeof(FuncArgs));
	printf("SetRoll size: %lu\n", sizeof(SetRoll));
	printf("SymbolRef size: %lu\n", sizeof(SymbolRef));
	printf("SymbolAssign size: %lu\n", sizeof(SymbolAssign));
	printf("Result size: %lu\n", sizeof(Result));
	printf("\n");
	#endif

	char *stdsave = "config.dei";

	FILE *f = fopen(stdsave, "r");
	if (f){
		// printf("found save!\n");
		yyin = f;
		yyparse();
		fclose(yyin);
		// printf("\n");
	}

	int i;
	for(i = 1; i < argc; i++) {
		f = fopen(argv[i], "r");

		if(!f) {
			perror(argv[i]);
			return (1);
		}

		// printf("new file!\n");
		yyin = f;
		yylineno = 1;
		yyrestart(yyin);
		yyparse();
		fclose(yyin);
		// printf("\n");
  }

	printf("\n> ");
	yyin = stdin;
	yylineno = 1;
	yyrestart(yyin);
  int ret = yyparse();
	freeTable();
	return ret;
}

void yyerror(char *s, ...){
  va_list ap;
  va_start(ap, s);

  fprintf(stderr, "%d: yy error : ", yylineno);
  vfprintf(stderr, s, ap);
  fprintf(stderr, "\n");
}
