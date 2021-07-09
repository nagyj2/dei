
#include <stdio.h> /* needed for FILE */
#include <assert.h> /* for assert */

#include "dei.tab.h"

#include "struct.h"
#include "symboltable.h"
#include "evaluation.h"
#include "util.h"

/* One line log statement for debugging */
#ifdef DEBUG
#define DEBUGLOG(text) fprintf(logger, "%s", text);
#else
#define DEBUGLOG(text)
#endif


/* ------------- UTILITY FUNC ------------- */

FILE *logger;
extern struct symbol symtab[];    /* symbol table itself */

void yyerror(char *s, ...){
  va_list ap;
  va_start(ap, s);

  fprintf(stderr, "%d: error : ", yylineno);
  vfprintf(stderr, s, ap);
  fprintf(stderr, "\n");
}


int mergeSelected(struct selected **sel1, struct selected *sel2){
  struct selected *t1 = NULL, *t2 = NULL;
  int num = 0; /* number of merges */

  if (!sel2) return num;  /* if sel2 is null, return */

  for (t2 = sel2; t2 && t2->val; t2 = t2->next){
    int missing = 1;
    for (t1 = *sel1; t1; t1 = t1->next){
      if (t1 == t2){ /* check for pointers to the same address */
        missing = 0;
      }
    }

    /* if not found, append */
    if (missing == 1){ /* if there was no match, append */
      num += 1;
      *sel1 = newSelected(t2->val, *sel1);
    }
  }

  if (sel2) freeSelected(sel2);
  return num;
}


struct selected *firstunique(struct value *opts, struct selected *has){
  struct value *p = NULL;

  for (p = opts; p; p = p->next){
      if (!hasSelected(p, has)){
        struct selected *t = malloc(sizeof(struct selected));
        t->val = p;
        return t;
      }
  }

  return NULL;
}

/* select elements of dieroll and stores them in special pointers for callbuiltin to operate on */
struct selected *select(struct selector *sele, struct roll *dieroll){
  struct selected *retsel = NULL, *sel = NULL; /* set to null so outside it can be seen if nothing was selected */
  struct value *t; /* traversal variable */

  int i = sele->count;
  if (i > countValue(dieroll->out)) printf("warning: requesting more selects than available");
  if (i == 0) return retsel;

  printf("<%d>",sele->sel);

  do {

    switch (sele->sel){
      case S_high: {
        sel = firstunique(dieroll->out, retsel); /* can't assume 1st without checking value b/c it wrecks with "sel->val->v > t->v && !hasSelected(t, retsel)" */
        if (!sel) break;
        //sel->val = dieroll->out;
        for (t = dieroll->out; t; t = t->next){ /* dont immediately go to next in case it is null */
          if (sel->val->v < t->v && !hasSelected(t, retsel)) {  /* saved -> initialized, so never null */
            sel->val = t; /* save address */
          }
        }
        assert(sel != NULL);
        break;
      }
      case S_low: {
        sel = firstunique(dieroll->out, retsel); /* can't assume 1st without checking value b/c it wrecks with "sel->val->v > t->v && !hasSelected(t, retsel)" */
        if (!sel) break;
        //sel->val = dieroll->out;
        for (t = dieroll->out; t; t = t->next){ /* dont immediately go to next in case it is null */
          if (sel->val->v > t->v && !hasSelected(t, retsel)) {  /* saved -> initialized, so never null */
            sel->val = t; /* save address */
          }
        }
        assert(sel != NULL);
        break;
      }
      case S_rand: {
        int index = randint(0, countValue(dieroll->out)-1); /* index which will be updated */
        for (t = dieroll->out; index-- > 0; t = t->next) { /* just need to update t */ }
        sel = newSelected(t, NULL);
        assert(sel != NULL);
        break;
      }
      case S_unique:
        /* cont holds list of unique values found */
        t = dieroll->out; /* start at the first roll */
        do {
          if (!sel || !hasSelectedInt(t->v, retsel)){
            /* initialize -> cannot init outside b/c it messes with hasValue */
            sel = newSelected(t, sel); /* cont will be null on first, starting the chain */
          }
          t = t->next;
        } while (t);
        assert(sel != NULL);
        break;
      default:
        /* find first instance of specific die outcome */
        assert(sele > 0);
        for (t = dieroll->out; t; t = t->next){
          if (t->v == sele->sel && !hasSelected(t, retsel)){  /* check for match */
            sel = newSelected(t, NULL);
            break; /* TODO: once per loop? */
          }
        }
        break;
    }

    //(!sel) break;
    if (sel){
      //if (!retsel) retsel = malloc(sizeof(struct selected));
      int merges = mergeSelected(&retsel, sel); /* merge current and newly found */
      if (merges == 0 && sele->sel != S_all)
        break;  /* short circuit of mergeSelected returns 0, so ensure nothing was actually entered */
    }else{
      break;
    }

  } while (--i > 0); /* stop when counting numbers, but continue */

  printf("selected:");
  for (sel = retsel; sel; sel = sel->next)
    printf(" %d",sel->val->v);
  printf("\n");

  if (t) free(t);
  return retsel;
}


/* rerolls all pointers pointed to by sel according to faces */
/* NEITHER sel or faces can be NULL */
void funcreroll(struct selected *sel, struct value *faces){
  /* length of faces  */
  int facelen = countValue(faces);

  struct selected *t; /* iterator variable */
  for (t = sel; t; t = t->next){
    int roll = randroll(faces);
    t->val->v = roll; /* update pointed to's value */
  }
}


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

  freesymboltable();

  return end;
}
