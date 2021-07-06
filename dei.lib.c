#include <stdio.h> /* needed for FILE */
#include <stdlib.h> /* needed for rand() */
#include <stdarg.h> /* needed for yyerror */
#include <string.h> /* needed for strdup() */
#include <math.h> /* needed for pow() */
#include <time.h> /* needed for time() */

#include "dei.tab.h"
#include "dei.h"

/* ------------- UTILITY FUNC ------------- */

/* generate a random number between min and max, inclusive */
int randint(int min, int max){
  return (rand() % (max - min + 1)) + min;
}

/* count values in a chain */
int countvalue(struct value *val){
  struct value *t;
  int c = 0;

  for (t = val; t != NULL; t=t->next) c++;
  return c;
}

/* sum values of a chain */
int sumvalue(struct value *val){
  struct value *t;
  int s = 0;

  for (t = val; t != NULL; t=t->next){
    s+=(t->v);
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

/* define a symbol (variable) */
void dosym(struct symbol *name, struct ast *val){
  if (name->func) treefree(name->func);
  name->func = val;
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
      return sp;
    }

    /* if no entry, make an entry and return it */
    if (!sp->name){
      sp->name = strdup(sym);
      sp->func = NULL;
      return sp;
    }

    /* if no return, try the next entry */
    /* increment the pointer and if out of bounds (goto next), loop to start of symtab */
    if (++sp >= symtab+NHASH) sp = symtab;
  }

  yyerror("symbol table overflow\n");
  abort(); /* tried all entries, table is full */
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
    printf("%s", ((struct symcall *)a)->s->name );
    break;

  case 'A':
    printf("%s", ((struct symasgn *)a)->s->name );
    printf(" ::= ");
    printtree( ((struct symasgn *)a)->l );
    break;

  }
}

/* recursively free ast nodes */
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

  /* 1 value subtree */
  case 'd': case 'Q':
    valuefree( ((struct setres *)a)->faces );
    break;

  /* special */
  /* function call -> requires cast */
  case 'F':
    treefree(((struct funcall *)a)->l);
    break;

  /* assignment -> free pointer to value; now stored by symbol table */
  case 'A':
    treefree( ((struct symasgn *)a)->l );
    break;

  default:
    printf("unknown nodetype: %c:", a->nodetype);
    return;

  }
  free(a); /* free node itself */
}

/* sequentially free chain of values */
void valuefree(struct value *a){
  struct value *na;
  while(a){
    na = a->next;
    free(a);
    a = na;
  }
}


#ifdef FIX

  /* ------------- AST EVALUATION ------------- */

  struct result *eval(struct ast *a){
    /* evaluates the result of ast node a */

    struct result* v = malloc(sizeof(struct result)); /* result value - set default */


    if (!v){
      yyerror("out of space");
      return 0;
    }
    if (!a){
      yyerror("internal error, null eval");
      return 0;
    }

    /* Node Types
     * + - * DIV %                    (ast)
     * < > <= >= == !=                (ast)
     *
     * M unary minus                  (ast)
     * D die roll result              (dieroll)
     * S set die rolls                (dieroll)
     * N symbol reference             (symref)
     * L expression or statement list (ast)
     * F built in function call       (fncall)
     * A symbol assignment            (symasgn)
     */

    switch (a->nodetype){

      /* a leaf integer */
    case 'I':
      v->type = R_int;
      v->ivalue = ((struct integer *)a)->value;
      break;

      /* a set of die rolls */
    case 'D':
      /* struct dieroll *r = (struct dieroll *)a; */  /* cast roll to proper struct */
      v->rvalue = malloc(sizeof(struct die));   /* create rolled output var */

      struct value *roll = NULL;
      int i;
      for (i = 0; i < ((struct dieroll *)a)->count; i++){       /* create roll chain */
        roll = addvalue(randint(((struct dieroll *)a)->used->min, ((struct dieroll *)a)->used->min), roll);
      }

      v->type = R_die;
      v->rvalue->faces = roll;        /* remember, in eval, faces is the roll result */
      v->rvalue->min = ((struct dieroll *)a)->used->min;
      v->rvalue->max = ((struct dieroll *)a)->used->max;
      v->rvalue->size = ((struct dieroll *)a)->count;    /* size of faces */
      break;

    case 'S':
      /* struct dieroll *r = (struct dieroll *)a; */   /* cast roll to proper struct */
      v->rvalue = malloc(sizeof(struct die));   /* create rolled output var */

      v->type = R_die;
      v->rvalue->faces = ((struct dieroll *)a)->used->faces;
      /* v->rvalue->min = NULL; r->used->min; */
      /* v->rvalue->max = NULL; r->used->max; */
      v->rvalue->size = ((struct dieroll *)a)->count;
      break;

      /* sum rolls */
    case 'Q':
      v->type = R_int;
      v->ivalue = sumvalue(((struct dieroll *)(a->l))->used->faces);
      break;

      /* symbol reference */
    case 'N':
      v = eval(((struct symref *)a)->s->func);
      break;

      /* assignment TODO: works? */
    case 'A':
      ((struct symasgn *)a)->s->func = ((struct symasgn *)a)->l;

      v->type = R_int;
      v->ivalue = 0;
      break;

      /* expressions */
    case '+':
      v->type = R_int;
      v->ivalue = eval(a->l)->ivalue + eval(a->r)->ivalue;
      break;
    case '-':
      v->type = R_int;
      v->ivalue = eval(a->l)->ivalue - eval(a->r)->ivalue;
      break;
    case '*':
      v->type = R_int;
      v->ivalue = eval(a->l)->ivalue * eval(a->r)->ivalue;
      break;
    case DIV:
      v->type = R_int;
      v->ivalue = eval(a->l)->ivalue / eval(a->r)->ivalue;
      break;
    case '%':
      v->type = R_int;
      v->ivalue = eval(a->l)->ivalue % eval(a->r)->ivalue;
      break;
    case '^':
      v->type = R_int;
      v->ivalue = (int) pow(eval(a->l)->ivalue, eval(a->r)->ivalue);
      break;
    case 'M':
      v->type = R_int;
      v->ivalue = - eval(a->l)->ivalue;
      break;

    case '&': v->type = R_int; v->ivalue = -2; break;
    case '|': v->type = R_int; v->ivalue = -3; break;
    case INTER: v->type = R_int; v->ivalue = -4; break;
    case UNION: v->type = R_int; v->ivalue = -5; break;

      /* comparisons */
    case '1': v->type = R_int; v->ivalue = (eval(a->l)->ivalue > eval(a->r)->ivalue)? 1 : 0; break;
    case '2': v->type = R_int; v->ivalue = (eval(a->l)->ivalue < eval(a->r)->ivalue)? 1 : 0; break;
    case '3': v->type = R_int; v->ivalue = (eval(a->l)->ivalue != eval(a->r)->ivalue)? 1 : 0; break;
    case '4': v->type = R_int; v->ivalue = (eval(a->l)->ivalue == eval(a->r)->ivalue)? 1 : 0; break;
    case '5': v->type = R_int; v->ivalue = (eval(a->l)->ivalue >= eval(a->r)->ivalue)? 1 : 0; break;
    case '6': v->type = R_int; v->ivalue = (eval(a->l)->ivalue <= eval(a->r)->ivalue)? 1 : 0; break;

      /* list of statements */
    /* case 'L': eval(a->l); v = eval(a->r); break; */

      /* function call */
      /* TODO: disect ops according to count->selector */
    case 'F': v->type = R_int; v->ivalue = -6; break; /*callbuiltin((struct fncall *)a);*/

    default: printf("internal error: bad node %c\n", a->nodetype);
    }
    return v;
  }

  /* call a user defined function and return the evaluation */
  static int calluser(struct ufncall *f){
    struct symbol *fn = f->s;  /* function name */
    struct symlist *sl;         /* dummy args */
    struct ast *args = f->l;    /* actual arguments */
    int *oldval, *newval;       /* saved arg values */
    int v;
    int nargs;
    int i;

    if (!fn->func){
      yyerror("call to undefined function", fn->name);
      return 0;
    }

    /* count args */
    sl = fn->syms;
    for (nargs = 0; sl; sl = sl->next)
      nargs++;

    /* prepare to save them */
    oldval = (int *) malloc(nargs * sizeof(int));
    newval = (int *) malloc(nargs * sizeof(int));
    if (!oldval || !newval){
      yyerror("out of space in %s", fn->name);
      return 0;
    }

    /* evaluate the arguments */
    for (i = 0; i < nargs; i++){
      if (!args){
        yyerror("too few args in call to %s", fn->name);
        free(oldval); free(newval); /* free b/c error was encountered */
        return 0;
      }

      if (args->nodetype == 'L'){ /* if this is a list node */
        newval[i] = eval(args->l);
        args = args->r;
      }else{                      /* if its the end of the list */
        newval[i] = eval(args);
        args = NULL;
      }
    }

    /* save old values of dummies, assign new ones */
    sl = fn->syms;
    for (i = 0; i < nargs; i++){
      struct symbol *s = sl->sym;

      oldval[i] = s->value;
      s->value = newval[i];
      sl = sl->next;
    }

    free(newval);

    /* evaluate function */
    v = eval(fn->func);

    /* put the real values of the dummies back */
    sl = fn->syms;
    for (i = 0; i < nargs; i++){
      struct symbol *s = sl->sym;

      s->value = oldval[i];
      sl = sl->next;
    }

    free(oldval);
    return v;
  }

  /* call a built in function */
  static int callbuiltin(struct fncall *f){

    enum bifs functype = f->functype;
    int v = eval(f->l);

    switch (functype){
    case B_sqrt:
      return (int) sqrt(v);
    case B_exp:
      return (int) exp(v);
    case B_log:
      return (int) log(v);
    case B_print:
      printf("= %d\n", v);
      return v;
    default:
      yyerror("Unknown built-in function %d", functype);
      return 0;
    }
  }

#endif


void yyerror(char *s, ...){
  va_list ap;
  va_start(ap, s);

  fprintf(stderr, "%d: error : ", yylineno);
  vfprintf(stderr, s, ap);
  fprintf(stderr, "\n");
}

int main(int argc, char **argv){
  #ifdef DEBUG
  yydebug = 1;
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

  return yyparse();
}
