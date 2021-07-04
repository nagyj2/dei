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
struct value *addvalue(int i, struct value *val){
  struct value *a = malloc(sizeof(struct value));

  if (!a){
    yyerror("out of space");
    exit(0);
  }

  a->v = i;
  if (val) a->next = val;
  return a;
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

/* create a new ast node */
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

/* create a built in function call node */
struct ast *newfunc(enum bifs functype, enum sifs selector, struct ast *base, int count){
  /* selector < 0 indicates a special selector was used -> sifs */
  /* functype -> bifs */

  struct fncall *a = malloc(sizeof(struct ast));

  if (!a){
    yyerror("out of space");
    exit(0);
  }

  a->nodetype = 'F';
  a->functype = functype;   /* type of function to perform */
  a->seltype = selector;    /* function argument (selector) */
  a->count = count;         /* how many times to perform function */
  a->l = base;              /* ast to perform function on */
  return (struct ast *)a;
}

/* create a symbol (variable) call node */
struct ast *newref(struct symbol *s){

  struct symref *a = malloc(sizeof(struct symref));

  if(!a) {
    yyerror("out of space");
    exit(0);
  }

  a->nodetype = 'N';
  a->s = s;
  return (struct ast *)a;
}

/* create symbol (variable) assignment node */
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

/* create symbol (variable) assignment node */
struct ast *newroll(int count, struct die *face){

  struct dieroll *a = malloc(sizeof(struct dieroll));

  if (!a){
    yyerror("out of space");
    exit(0);
  }

  a->nodetype = 'D';      /* eval -> treat used as die faces for a roll */
  a->used = face;         /* die faces to use for roll generation */
  a->count = count;       /* how many rolls to perform */
  return (struct ast *)a;
}

/* set a roll face node to a roll result */
struct ast *setroll(struct die *rolls){

  struct dieroll *a = malloc(sizeof(struct dieroll));

  if (!a){
    yyerror("out of space");
    exit(0);
  }

  a->nodetype = 'S';      /* eval -> treat used as rolls */
  a->used = rolls;        /* none because setroll doesnt use a random r */
  a->count = rolls->size; /* number of rolls */
  return (struct ast *)a;

}

/* create a leaf node from an integer */
struct ast *newint(int value){
  struct integer *a = malloc(sizeof(struct integer));

  if (!a){
    yyerror("out of space");
    exit(0);
  }

  a->nodetype = 'I';
  a->value = value;
  return (struct ast *)a;
}

/* create a leaf node */
struct ast *newsum(struct ast *roll){
  struct ast *a = malloc(sizeof(struct ast));

  if (!a){
    yyerror("out of space");
    exit(0);
  }

  a->nodetype = 'Q';
  a->l = roll;
  return a;
}

/* create a rollface from a face max */
struct die *newdie(int min, int max){

  if (max <= min){
    yyerror("invalid die");
    exit(0);
  }

  struct die *d = malloc(sizeof(struct die));
  struct value *a = NULL;
  if (!d){
    yyerror("out of space");
    exit(0);
  }

  int i;
  for(i = max; i >= min; i--) {                     /* start at max so output a is the lowest */
    struct value *b = malloc(sizeof(struct value)); /* will become the tail */

    if (!b){
      yyerror("out of space");
      exit(0);
    }

    /* fill out new value node */
    b->v = i;
    b->next = a;
    a = b;
  }

  d->faces = a;
  d->size = max - min + 1; /* +1 since max is inclusive */
  d->max = max;
  d->min = min;
  return d;
}

/* set the die faces to be a link of  roll face */
struct die *setdie(struct value *rolls){

  struct die *d = malloc(sizeof(struct die));

  if (!d){
    yyerror("out of space");
    exit(0);
  }

  d->faces = rolls;
  d->size = countvalue(rolls);   /* count of faces */
  /* d->max = NULL; invalid since there was no die roll */
  /* d->min = NULL; invalid since there was no die roll */
  return d;
}


/* recursively free dice */
void diefree(struct die *a){
  struct value *t = a->faces, *p = a->faces;  /* current node and previous node */
  /* set previous to start node since t is set to 2nd place immediately */
  for (t = t->next; t; t = t->next){
    free(p);
    p = t;
  }
  free(a);
}

/* recursively free ast nodes */
void treefree(struct ast *a){

  switch (a->nodetype){
    /* two subtrees */
  case '+': case '-':
  case '*': case DIV: case '%':
  case '^':
  case '1': case '2': case '3': case '4': case '5': case '6': /* CMP */
  case 'L':
    treefree(a->r);

    /* one subtree */
  case 'F': case 'M': case 'A': case 'Q':
    treefree(a->l);

    /* no subtrees */
  case 'N': case 'I':
    break;

    /* special */
      /* cast to symasgn to access v and free it */
  /*case 'A':
    free( ((struct symasgn *)a)->v );
    break;*/

  case 'D': case 'S':
    diefree( ((struct dieroll *)a)->used );
    break;

  default: printf("internal error: free bad node %c\n", a->nodetype);
  }

  free(a); /* free node itself */
}


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

#ifdef FIX

/* define a symbol (variable) */
void dosym(struct symbol *name, struct ast *val){
  if (name->func) treefree(name->func);
  name->func = val;
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

/*


Codesigning requires a certificate. The following procedure explains how to create one and apply it to gdb:

    Start the Keychain Access application (in /Applications/Utilities/Keychain Access.app)
    Select the Keychain Access -> Certificate Assistant -> Create a Certificate... menu
    Then:
    Choose a name for the new certificate (this procedure will use "gdb-cert" as an example)
    Set "Identity Type" to "Self Signed Root"
    Set "Certificate Type" to "Code Signing"
    Activate the "Let me override defaults" option
    Click several times on "Continue" until the "Specify a Location For The Certificate" screen appears, then set "Keychain" to "System"
    Click on "Continue" until the certificate is created
    Finally, in the view, double-click on the new certificate, and set "When using this certificate" to "Always Trust"
    Exit the Keychain Access application and restart the computer (this is unfortunately required)
    Once a certificate has been created, the debugger can be codesigned as follow. In a Terminal, run the following command:

$ codesign -fs gdb-cert /path/to/gdb
where "gdb-cert" should be replaced by the actual certificate name chosen above.

When run as sudo, gdb will no longer report the taskgated error.
*/
