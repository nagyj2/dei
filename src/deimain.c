
#include <stdarg.h> /* needed for yyerror */
#include <assert.h>

#include "deimain.h"
#include "struct.h"

void test_value(){

	#define MAX 10
	#define MIN -1
	#define LEN ((MAX - MIN - 1) / 1 + 1)

	struct value *val1 = newValueChain(MIN,MAX);

	reverseValue( &val1 ); /* reverse direction so it goes low -> high */

	struct value *val2 = dupValue( val1 );

	struct value *t1 = NULL, *t2 = NULL;
	int c = 0, s = 0, i = MAX;
	for (t1 = val1, t2 = val2; t1 || t2; t1 = t1->next, t2 = t2->next){
		assert(t1->i == i); /* Assert values were copied */
		assert(t2->i == i);
		assert(!hasValueExact(t2, val1)); /* Assert values were not aliased */
		s+=t1->i;	/* ongoing sum */
		c++; /* ongoing count */
		i--; /* what num should be */
	}

	assert( countValue(val1) == c ); /* Assert count is correct */
	assert( sumValue(val2) == s );

	i = MAX-(int)(LEN/2);
	t1 = backValue(val1);
	t2 = findValue(i, val1);

	assert(hasValueExact(t2, val1)); /* 1. Assert value taken halfway is aliased */
	assert(hasValue(t2->i, val2)); /* 2. Assert val2 has the key... */
	assert(!hasValueExact(t2, val2)); /* 3. ...but not the exact reference */

	assert(hasValueExact(t1, val1)); /* 4. Assert value taken halfway is aliased */
	assert(hasValue(t1->i, val2)); /* 5. Assert val2 has the key... */
	assert(!hasValueExact(t1, val2)); /*6.  ...but not the exact reference */

	t1 = removeValue(t1, &val1); /* 7. Remove exact reference and return it */
	assert(!hasValueExact(t1, val1)); /* 8.  Assert value has been removed... */
	assert(hasValue(t1->i, val2)); /* 9. ... but not from val */
	assert(!hasValueExact(t1, val2)); /* 10. assert that it still doesnt have exact reference */

	assert( countValue(val1) == c - 1 ); /* Assert count is correct */
	assert( countValue(val2) == c );

	assert ( !findValue(t1->i, val1) ); /* assert unfound */

	t2 = popValue(t1->i, &val2); /* remove the same numbered value */
	assert(!hasValue(t2->i, val1)); /* assert no one has the value */
	assert(!hasValue(t2->i, val2));
	assert(!hasValueExact(t2, val1));
	assert(!hasValueExact(t2, val2));

	assert( countValue(val1) == c - 1 ); /* Assert count is still correct */
	assert( countValue(val2) == c - 1 );

	assert ( !findValue(t2->i, val2) ); /* assert unfound */

	freeValue( &t1 );
	freeValue( &t2 );

	assert( countValue(val1) == c - 1 ); /* ensure still decoupled */
	assert( countValue(val2) == c - 1 );
	assert(!t1 && !t2);

	t1 = copyValue(findValue(i+1, val1)); /* create unattached value */
	assert(hasValue(t1->i, val1)); /* Assert has value, but not exact */
	assert(hasValue(t1->i, val2));
	assert(!hasValueExact(t1, val1));
	assert(!hasValueExact(t1, val2));

	freeValue( &t1 );

	freeValue( &val1 );
	freeValue( &val2 );

	assert(!val1 && !val2); /* assert free will unassign vars */
	assert( countValue(val1) == 0 ); /* assert correct default behaviour */
	assert( sumValue(val1) == 0 );
}

int main(int argc, char **argv){
	test_value();
	return 0;

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
