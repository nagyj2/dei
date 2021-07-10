
#include "dei.tab.h"

#include "deimain.h"


int main(int argc, char **argv){
  #ifdef DEBUG
  #ifdef YYDEBUG
  yydebug = 1;
  #endif

  logger = fopen("dei.log", "a+"); // a+ (create + append) option will allow appending which is useful in a log file
  if (logger == NULL) { perror("Failed: "); return 1; }

  time_t rawtime;
  struct tm *timeinfo;
  time(&rawtime);
  timeinfo = localtime(&rawtime);

  fprintf(logger, "=== %s  ===\n", asctime(timeinfo));


  printf("value bytes: %lu\n", sizeof(struct value));
  printf("symbol bytes: %lu\n", sizeof(struct symbol));
  printf("die bytes: %lu\n", sizeof(struct die));
  printf("roll bytes: %lu\n", sizeof(struct roll));
  printf("result bytes: %lu\n", sizeof(struct result));
  printf("\n");
  printf("ast bytes: %lu\n", sizeof(struct ast));
  printf("natdie bytes: %lu\n", sizeof(struct natdie));
  printf("setdie bytes: %lu\n", sizeof(struct setdie));
  printf("natint bytes: %lu\n", sizeof(struct natint));
  printf("setres bytes: %lu\n", sizeof(struct setres));
  printf("funcall bytes: %lu\n", sizeof(struct funcall));
  printf("symcall bytes: %lu\n", sizeof(struct symcall));
  printf("symasgn bytes: %lu\n\n", sizeof(struct symasgn));

  #endif

  /* seed rng with current time */
  srand(time(NULL));

  if (argc > 1){
    if (!(yyin = fopen(argv[1], "r"))){
      perror(argv[1]);
      return 1;
    }
  }else{
    printf("> ");
  }

  int end = yyparse();

  //freesymboltable();

  return end;
}
