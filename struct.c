
#include "struct.h"

/* === Data Functions === */

/* append a new value node to the beginning of a chain */
struct value *newValue(int elem, struct value *prev){
  struct value *a = malloc(sizeof(struct value));

  if (!a){
    yyerror("out of space");
    exit(0);
  }

  a->v = elem;
  if (prev) a->next = prev;
  else      a->next = NULL;
  return a;
}

/* count values in a chain */
int countValue(struct value *val){
  struct value *t;
  int c = 0;

  for (t = val; t; t=t->next) c++;
  /*printf("count %d\n",c);*/
  return c;
}

/* sum values of a chain */
int sumValue(struct value *val){
  struct value *t;
  int s = 0;

  for (t = val; t && t->v; t = t->next){
    s += (t->v);
  }

  return s;
}

/* check if key is in list's CHAIN */
bool hasValue(int key, struct value *list){
  if (!list) return false;

  struct value *t;
  for (t = list; t; t = t->next)
    if (t->v == key)
      return true;

  return false;
}


/* === AST Node Creation === */

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
  //DEBUGLOG("new %c node. l at %p, r at %p\n", a->nodetype, a->l, a->r);
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
  //DEBUGLOG("new cmp %c node. l at %p, r at %p\n", '0' + a->nodetype, a->l, a->r);
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
  //DEBUGLOG("new nat die: %dd{%d..%d}\n", a->count, a->min, a->max);

  return (struct ast *)a;
}

/* d : create a artificial face die */
struct ast *newsetdie(int count, struct value *faces){
  struct setdie *a = malloc(sizeof(struct setdie));

  if (!a){
    yyerror("out of space");
    exit(0);
  }

  a->nodetype = 'd';
  a->count = count;
  a->faces = faces;
  /*DEBUGLOG("new set die: ");
  #ifdef DEBUG
  struct value *t;
  for (t = faces; t; t = t->next);
    DEBUGLOG("%d ")
  DEBUGLOG("\n");
  #endif*/
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
struct ast *newfunc(int functype, struct selector *selectype, int times, struct ast *l){
  struct funcall *a = malloc(sizeof(struct funcall));

  if (!a){
    yyerror("out of space");
    exit(0);
  }

  a->nodetype = 'F';
  a->times = times;
  a->functype = functype;
  a->sel = selectype;
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
struct ast *newasgn(struct symbol *s, struct ast *defn){
  struct symasgn *a = malloc(sizeof(struct symasgn));

  if (!a){
    yyerror("out of space");
    exit(0);
  }

  a->nodetype = 'A';
  a->s = s;           /* symbol from symbol table */
  a->l = defn;        /* the unevaluated ast which the symbol refers to */
  return (struct ast *)a;
}


/* === Memory Management === */

/* sequentially free chain of values */
void freeValue(struct value *a){
  struct value *na;
  while(a != NULL){
    na = a->next;
    free(a);
    a = na;
  }
}

void freeAst(struct ast *a){

  switch (a->nodetype){

  /* 2 ast subtrees */
  case '+': case '-': case '*': case '%': case '^': case '&': case '|':
  case DIV: case INTER: case UNION:
  case '1': case '2': case '3': case '4': case '5': case '6':
    freeAst(a->r);

  /* 1 ast subtrees */
  case 'M': case 'R': case 'r': case 'S':
    freeAst(a->l);

  /* 0 ast subtrees */
  case 'D': case 'I': case 'E':
    break;

    /* special */
  /* 1 value subtree */
  case 'd':
    freeValue( ((struct setdie *)a)->faces );
    break;

  case 'Q':
    freeValue( ((struct setres *)a)->faces );
    break;

  /* function call -> requires cast */
  case 'F':
    freeAst(((struct funcall *)a)->l);
    break;

  /* assignment -> free pointer to value; now stored by symbol table */
  case 'A':
    freeAst( ((struct symasgn *)a)->l );
    break;

  default:;
    //printf("unknown nodetype: %d:", a->nodetype);

  }
  free(a); /* free node itself */
}
