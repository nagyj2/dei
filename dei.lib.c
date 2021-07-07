#include <stdio.h> /* needed for FILE */
#include <stdlib.h> /* needed for rand() */
#include <stdarg.h> /* needed for yyerror */
#include <string.h> /* needed for strdup() */
#include <math.h> /* needed for pow() */
#include <time.h> /* needed for time() */
#include <stdbool.h> /* needed for bool, true, false */

#include "dei.tab.h"
#include "dei.h"

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

/* count values in a chain */
int countvalue(struct value *val){
  struct value *t;
  int c = 0;

  for (t = val; t->next != NULL; t=t->next) c++;
  //printf("length %d\n",c);
  return c;
}

/* generate a random number between min and max, inclusive */
/*int randinti(int min, int max){
  return (rand() % (max - min + 1)) + min;
}*/

/* generate a random number within a set of faces */
int randroll(struct value *faces){
  struct value *t = faces;
  int index, len = countvalue(faces);
  for (index = rand() % len; index > 0; index--)
    t = t->next;
  return t->v;
}

/* sum values of a chain */
int sumvalue(struct value *val){
  struct value *t;
  int s = 0;

  for (t = val; t; t = t->next){
    s += (t->v);
  }

  return s;
}

/* append a new value node to the beginning of a chain */
struct value *newvalue(int i, struct value *val){
  struct value *a = malloc(sizeof(struct value));

  if (!a){
    yyerror("out of space");
    exit(0);
  }

  a->v = i;
  if (val) a->next = val;
  return a;
}

/* check if key is in list's CHAIN */
bool containvalue(struct value *key, struct value *list){
  struct value *t;

  for (t = list; t != NULL; t = t->next){
    if (t->v == key->v) return true;
  }

  return false;
}


/* --------- UTILITY EVAL FUNCTIONS --------- */

/* create the faces of a natural die for result */
struct value *createnatdieface(int min, int max){
  if (min>max) yyerror("invalid die, %d,%d", min, max);
  struct value *a = newvalue(min,NULL);
  int i;
  for (i = min + 1; i <= max; i++)
    a = newvalue(i,a);
  return a;
}

/* create the faces of a set die for result */
struct value *createsetdieface(struct value *a){
  if (!a) { yyerror("invalid set die"); return newvalue(0,NULL); }
  return a;
}

/* find intersection and append intersecting to a */
struct roll *evalElemInter(struct roll *a, struct roll *b){
  struct roll *r;   /* output  */
  struct value *t;

  r = a; /* set r to a initially */

  for (t = a->out; t != NULL; t = t->next)
    if (containvalue(t, b->out))
      r->out = newvalue(t->v, a->out);

  /* TODO : Need to use resultfree */
  // free(a);  /* WARNING : Side effect is freeing args */
  // free(b);  /* DEBUG : does freeing a leave r? */
  return r;
}

/* find union and append intersecting to a */
struct roll *evalElemUnion(struct roll *a, struct roll *b){
  struct roll *r;   /* output  */
  struct value *t;

  r = a; /* set r to a initially */

  for (t = a->out; t != NULL; t = t->next)
    r->out = newvalue(t->v, a->out);

  /* TODO : Need to use resultfree */
  // free(a);  /* WARNING : Side effect is freeing args */
  // free(b);  /* DEBUG : does freeing a leave r? */
  return r;
}

/* find intersection of two operands */
struct roll *evalSetInter(struct roll *a, struct roll *b){
  struct roll *r = malloc(sizeof(struct roll));   /* output  */
  struct value *t;

  for (t = a->out; t != NULL; t = t->next)
    if (containvalue(t, b->out))
      r->out = newvalue(t->v, a->out);

  /* TODO : Need to use resultfree */
  // free(a);  /* WARNING : Side effect is freeing args */
  // free(b);
  return r;
}

/* find union of two operands */
struct roll *evalSetUnion(struct roll *a, struct roll *b){
  struct roll *r;   /* output  */
  struct value *t;

  r = a; /* set r to a initially */

  for (t = a->out; t != NULL; t = t->next)
    if (containvalue(t, b->out) && !containvalue(t,r->out))
      r->out = newvalue(t->v, a->out);

  /* TODO : Need to use resultfree */
  // free(a);  /* WARNING : Side effect is freeing args */
  // free(b);  /* DEBUG : does freeing a leave r? */
  return r;
}

/* roll a natural die */
/*
struct roll *evalNatRoll(struct die *d){
  struct roll *r = malloc(sizeof(struct roll));
  int i;
  for (i = 0; i < d->count; i++){
    r->out = newvalue(randint(d->faces),r->out);
  }
  r->faces = d->faces;
  return r;
}
*/

/* roll a special die */
/*
struct roll *evalSetRoll(struct die *d){
  struct roll *r = malloc(sizeof(struct roll));
  int i;
  for (i = 0; i < d->count; i++){
    r->out = newvalue(randint(d->faces),r->out);
  }
  r->faces = d->faces;
  return r;
}
*/

/* sum results of a roll */
int evalSum(struct roll *r){
  int i = sumvalue(r->out);
  return i;
}


/* ----------- SYMBOL TABLE FUNCS ----------- */

/* hash a symbol to produce an index to place in symbol table at */
static unsigned symhash(char *sym){

  unsigned int hash = 0;
  unsigned c;

  while ((c = *sym++)) hash = hash*9 ^ c;
  return hash;
}

/* given a character symbol, return the symbol value from the symbol table */
struct symbol *lookup(char * sym){

  struct symbol *sp = &symtab[symhash(sym)%NHASH]; /* symtab position's contents */
  int scount = NHASH; /* how many slots we have yet to look at */

  while (--scount >= 0){
    /* if entry exists, check if it is the same and return if it is */
    if (sp->name && !strcmp(sp->name, sym)) {

      /* printf("found %s at %p\n", sp->name, sp); */
      return sp;
    }

    /* if no entry, make an entry and return it */
    if (!sp->name){
      sp->name = strdup(sym);
      sp->func = malloc(sizeof(struct ast));
      sp->func = newnatint(0);                /* initialize to protect against errors */
      /* printf("new %s at %p(%p)\n", sp->name, sp, sp->func); */
      return sp;
    }

    /* if no return, try the next entry */
    /* increment the pointer and if out of bounds (goto next), loop to start of symtab */
    if (++sp >= symtab+NHASH) sp = symtab;
  }

  yyerror("symbol table overflow\n");
  abort(); /* tried all entries, table is full */
}

/* define a symbol (variable) */
void setsym(struct symbol *name, struct ast *val){
  /* printf("place at %p\n", name); */
  if (name->func){ /* NOTE allocated in lookup, so this will always run*/
    treefree(name->func);
    name->func = malloc(sizeof(struct ast));
  }

  name->func = val;
}


void printsymtab(){
  int i;
  printf("Start table>");
  for (i=0; i<NHASH; i++){
    struct symbol *sp = &symtab[i];
    if (sp->name)
      printf("\t> %s : ", sp->name);
      printtree(sp->func);
      printf("\n");
  }
  printf("======\n");
}


/* ----------- AST CREATION FUNCS ----------- */

/* R r S : create a new ast node */
struct ast *newast(int nodetype, struct ast *l, struct ast *r){
  struct ast *a = malloc(sizeof(struct ast));

  if (!a){
    yyerror("out of space");
    exit(0);
  }

  a->nodetype = nodetype;
  a->l = l;
  a->r = r;
  return a;
}

/* create a comparison type node */
struct ast *newcmp(int cmptype, struct ast *l, struct ast *r){
  struct ast *a = malloc(sizeof(struct ast));

  if (!a){
    yyerror("out of space");
    exit(0);
  }

  a->nodetype = '0' + cmptype;
  a->l = l;
  a->r = r;
  return a;
}

/* D : create a natural die */
struct ast *newnatdie(int count, int min, int max){
  struct natdie *a = malloc(sizeof(struct natdie));

  if (!a){
    yyerror("out of space");
    exit(0);
  }

  a->nodetype = 'D';
  a->count = count;
  a->min = min;
  a->max = max;

  return (struct ast *)a;
}

/* d : create a special face die */
struct ast *newsetdie(int count, struct value *faces){
  struct setdie *a = malloc(sizeof(struct setdie));

  if (!a){
    yyerror("out of space");
    exit(0);
  }

  a->nodetype = 'd';
  a->count = count;
  a->faces = faces;

  return (struct ast *)a;
}

/* Q : create a node to roll the contained die */
struct ast *newsetres(struct value *faces){
  struct setres *a = malloc(sizeof(struct setres)); /* return value */

  if (!a){
    yyerror("out of space");
    exit(0);
  }

  a->nodetype = 'Q';
  a->faces = faces;
  return (struct ast *)a;
}

/* F : create a built in function call node */
struct ast *newfunc(int functype, int selectype, int times, struct ast *l){
  struct funcall *a = malloc(sizeof(struct funcall));

  if (!a){
    yyerror("out of space");
    exit(0);
  }

  a->nodetype = 'F';
  a->times = times;
  a->functype = functype;
  a->selector = selectype;
  a->l = l;                 /* operand */
  return (struct ast *)a;
}

/* I : create a natural integer */
struct ast *newnatint(int integer){
  struct natint *a = malloc(sizeof(struct natint)); /* return value */

  if (!a){
    yyerror("out of space");
    exit(0);
  }

  a->nodetype = 'I';
  a->integer = integer;
  return (struct ast *)a;
}

/* E : create a symbol (variable) call node */
struct ast *newref(struct symbol *s){
  struct symcall *a = malloc(sizeof(struct symcall));

  if(!a) {
    yyerror("out of space");
    exit(0);
  }

  a->nodetype = 'E';
  a->s = s;
  return (struct ast *)a;
}

/* A : create symbol (variable) assignment node */
struct ast *newasgn(struct symbol *s, struct ast *meaning){
  struct symasgn *a = malloc(sizeof(struct symasgn));

  if (!a){
    yyerror("out of space");
    exit(0);
  }

  a->nodetype = 'A';
  a->s = s;                 /* symbol from symbol table */
  a->l = meaning;           /* the unevaluated ast which the symbol refers to */
  return (struct ast *)a;
}


/* ------------- AST EVALUATION ------------- */

/* print the ast to stdout */
void printtree(struct ast *a){
  switch (a->nodetype){

  case '+': case '-': case '*': case '%': case '^': case '&': case '|':
    printf("(");/*printf("%c(", a->nodetype);*/
    printtree(a->l);
    printf("%c",a->nodetype);/*printf(",");*/
    printtree(a->r);
    printf(")");
    break;

  case 'M': case 'R': case 'r': case 'S':
    printf("%c(", a->nodetype);
    printtree(a->l);
    printf(")");
    break;

  case DIV: case INTER: case UNION:
    printf("(");/*printf("//(");*/
    printtree(a->l);
    printf("%s",(a->nodetype==DIV?"//":a->nodetype==INTER?"&&":"||"));/*printf(",");*/
    printtree(a->r);
    printf(")");
    break;

  case '1': /* > */
    printf("(");
    printtree(a->l);
    printf(">");
    printtree(a->r);
    printf(")");
    break;

  case '2': /* < */
    printf("(");
    printtree(a->l);
    printf("<");
    printtree(a->r);
    printf(")");
    break;

  case '3': /* != */
    printf("(");
    printtree(a->l);
    printf("!=");
    printtree(a->r);
    printf(")");
    break;

  case '4': /* == */
    printf("(");
    printtree(a->l);
    printf("==");
    printtree(a->r);
    printf(")");
    break;

  case '5': /* >= */
    printf("(");
    printtree(a->l);
    printf(">=");
    printtree(a->r);
    printf(")");
    break;

  case '6': /* <= */
    printf("(");
    printtree(a->l);
    printf("<=");
    printtree(a->r);
    printf(")");
    break;

  case 'D':
    printf("{%d,%d}x%d",((struct natdie *)a)->min, ((struct natdie *)a)->max, ((struct natdie *)a)->count);
    break;

  case 'd':{
    struct value *t = ((struct setres *)a)->faces;
    printf("{%d", t->v);
    for (t = t->next; t; t = t->next) printf("|%d", t->v);
    printf("}");
    free(t);
    break;
    }

  case 'Q':{
    struct value *t = ((struct setres *)a)->faces;
    printf("[%d", t->v);
    for (t = t->next; t; t = t->next) printf("|%d", (t->v));
    printf("]");
    free(t);
    break;
    }

  case 'F':
    printf("F(%d,%d,(",
      ((struct funcall *)a)->functype,
      ((struct funcall *)a)->selector
    );
    printtree(((struct funcall *)a)->l);
    printf(")x%d)", ((struct funcall *)a)->times);
    break;

  case 'I':
    printf("%d",((struct natint *)a)->integer);
    break;

  case 'E':
    printf("<%s:", ((struct symcall *)a)->s->name );
    printtree( ((struct symcall *)a)->s->func );
    printf(">");
    break;

  case 'A':
    printf("%s", ((struct symasgn *)a)->s->name );
    printf(" ::= ");
    printtree( ((struct symasgn *)a)->l );
    break;

  }
}

/* sequentially free chain of values */
void valuefree(struct value *a){
  struct value *na;
  while(a != NULL){
    na = a->next;
    free(a);
    a = na;
  }
}

void resultfree(struct result *a){
  switch (a->type){
  case R_int:
    break;

  case R_roll:
    if (a->r->faces)
      valuefree(a->r->faces);
    if (a->r->out)
      valuefree(a->r->out);
    break;

  case R_die:
    if (a->d->faces)
      valuefree(a->d->faces);
    break;

  default:
    printf("unrecognized result: %c",a->type);
  }
  free(a);
}

void treefree(struct ast *a){

  switch (a->nodetype){

  /* 2 ast subtrees */
  case '+': case '-': case '*': case '%': case '^': case '&': case '|':
  case DIV: case INTER: case UNION:
  case '1': case '2': case '3': case '4': case '5': case '6':
    treefree(a->r);

  /* 1 ast subtrees */
  case 'M': case 'R': case 'r': case 'S':
    treefree(a->l);

  /* 0 ast subtrees */
  case 'D': case 'I': case 'E':
    break;

    /* special */
  /* 1 value subtree */
  case 'd':
    valuefree( ((struct setdie *)a)->faces );
    break;

  case 'Q':
    valuefree( ((struct setres *)a)->faces );
    break;

  /* function call -> requires cast */
  case 'F':
    treefree(((struct funcall *)a)->l);
    break;

  /* assignment -> free pointer to value; now stored by symbol table */
  case 'A':
    treefree( ((struct symasgn *)a)->l );
    break;

  default:;
    //printf("unknown nodetype: %d:", a->nodetype);

  }
  free(a); /* free node itself */
}

struct result *eval(struct ast *a){
  if (!a){
    yyerror("internal error, null eval");
    return 0;
  }

  struct result* v = malloc(sizeof(struct result)); /* result value - set default */
  if (!v){
    yyerror("out of space");
    return 0;
  }

  switch (a->nodetype){

      /* Leaf Nodes */
      /* a leaf integer */
    case 'I':
      v->type = R_int;
      v->i = ((struct natint *)a)->integer;
      break;

    case '+': case '-': case '*': case DIV: case '%': case '^': {
      v->type = R_int;
      struct result *l = eval(a->l);
      struct result *r = eval(a->r);

      if (l->type != R_int || r->type != R_int){
        yyerror("integer nodes expected! got %c, %c",l->type,r->type);
        exit(0);
      }

      switch (a->nodetype){
        case '+': v->i = l->i + r->i; break;
        case '-': v->i = l->i - r->i; break;
        case '*': v->i = l->i * r->i; break;
        case DIV: v->i = (int) (l->i / r->i); break;
        case '%': v->i = l->i % r->i; break;
        case '^': v->i = (int) pow(l->i, r->i); break;
      }

      resultfree(l);
      resultfree(r);
      break;
    }

    case '1': case '2': case '3': case '4': case '5': case '6': {
      v->type = R_int;
      struct result *l = eval(a->l);
      struct result *r = eval(a->r);

      if (l->type != R_int || r->type != R_int){
        yyerror("integer nodes expected! got %c, %c",l->type,r->type);
        exit(0);
      }

      switch (a->nodetype){
        case '1': v->i = (l->i > r->i) ? 1 : 0; break;
        case '2': v->i = (l->i < r->i) ? 1 : 0; break;
        case '3': v->i = (l->i != r->i)? 1 : 0; break;
        case '4': v->i = (l->i == r->i)? 1 : 0; break;
        case '5': v->i = (l->i >= r->i)? 1 : 0; break;
        case '6': v->i = (l->i <= r->i)? 1 : 0; break;
      }

      resultfree(l);
      resultfree(r);
      break;
    }

    case 'Q':
      v->type = R_roll;
      v->r = malloc(sizeof(struct roll));       /* need to malloc space */
      v->r->out = ((struct setres *)a)->faces;
      /* v->r->faces = NULL */
      break;

    case 'S': {
      v->type = R_int;
      struct result *r = eval(a->l);

      if (r->type != R_roll){
        yyerror("roll node expected! got %c",r->type);
        exit(0);
      }

      v->i = sumvalue(r->r->out);
      /* v->r->faces = NULL */
      break;
    }

    case 'D':
      v->type = R_die;
      v->d = malloc(sizeof(struct die));
      v->d->count = ((struct natdie *)a)->count;
      v->d->faces = createnatdieface( ((struct natdie *)a)->min, ((struct natdie *)a)->max );
      break;

    case 'R': {
      v->type = R_roll;
      struct result *r = eval(a->l);

      if (r->type != R_die){
        yyerror("die node expected! got %c",r->type);
        exit(0);
      }

      v->r = malloc(sizeof(struct roll));
      int i, length = countvalue(r->d->faces);

      for (i = 0; i < r->d->count; i++)
        v->r->out = newvalue(randroll(length, r->d->faces), v->r->out);
      v->r->faces = r->d->faces;

      //resultfree(r);
      free(r);
      break;
    }

    case 'E':
      v->type = R_int;
      v->i = eval( (((struct symcall *)a)->s)->func )->i;
      break;

    case 'A':
      setsym( ((struct symasgn *)a)->s, ((struct symasgn *)a)->l );
      /*printf("stored %s as ", ((struct symasgn *)a)->s->name);
      printtree(((struct symasgn *)a)->s->func);
      printf("\n");*/
      v->type = R_int;
      v->i = 0; //eval( ((struct symasgn *)a)->s->func );
      break;

    default:
      printf("unrecognized ast: >%c<\n", a->nodetype);
  }

  return v;
}

int main(int argc, char **argv){
  #ifdef DEBUG
  yydebug = 1;
  #endif

  logger = fopen("dei.log", "a+"); // a+ (create + append) option will allow appending which is useful in a log file
  if (logger == NULL) { perror("Failed: "); return 1; }

  time_t rawtime;
  struct tm *timeinfo;
  time(&rawtime);
  timeinfo = localtime(&rawtime);

  fprintf(logger, "=== %s  ===\n", asctime(timeinfo));

  /* seed rng with current time */
  srand(time(NULL));

  /* allocate memory for symbol table */
  // struct symbol *symtab = malloc(NHASH * sizeof(struct symbol));


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
