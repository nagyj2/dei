
#include <stdlib.h>
#include <stdio.h>
#include <stdarg.h>

#include "deimain.h"

int main(int argc, char **argv){

	return 0;
}

void yyerror(char *s, ...){
  va_list ap;
  va_start(ap, s);

  fprintf(stderr, "%d: error : ", yylineno);
  vfprintf(stderr, s, ap);
  fprintf(stderr, "\n");
}
