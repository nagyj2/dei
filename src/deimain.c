
#include <stdarg.h> /* needed for yyerror */
#include <assert.h>
#include <time.h> /* time */

#include "deimain.h"
#include "struct.h"
#include "ast.h"

#include "eval.h"

#ifdef TESTS
#include "tests.h"
#endif

int main(int argc, char **argv){

	/* seed random number generator */
	srand(time(NULL));

	#ifdef TESTS
	test_value();
	return 0;
	#endif

	#ifdef DEBUG
	printf("value size: %lu\n", sizeof(struct value));
	printf("symbol size: %lu\n", sizeof(struct symbol));
	printf("ast size: %lu\n", sizeof(struct ast));
	printf("natdie size: %lu\n", sizeof(struct natdie));
	printf("setdie size: %lu\n", sizeof(struct setdie));
	printf("natint size: %lu\n", sizeof(struct natint));
	printf("fargs size: %lu\n", sizeof(struct fargs));
	printf("setres size: %lu\n", sizeof(struct setres));
	printf("symcall size: %lu\n", sizeof(struct symcall));
	printf("astAsgn size: %lu\n", sizeof(struct astAsgn));
	printf("result size: %lu\n", sizeof(struct result));
	printf("\n");
	#endif

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
