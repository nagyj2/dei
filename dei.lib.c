#include <stdio.h>
#include <stdlib.h>
#include <stdarg.h>

#include "dei.tab.h"
#include "dei.h"

/*
#define YYDEBUG
*/

/* utility functions for lexer and parser */

struct ast *newast(int nodetype, struct ast *l, struct ast *r){
  struct ast *a = malloc(sizeof(struct ast)); /* allocate new node */

  if (!a){
    yyerror("out of space");
    exit(0);
  }

  a->nodetype = nodetype;
  a->l = l;
  a->r = r;
  return a;
}

struct ast *newnum(int d){
  struct numval *a = malloc(sizeof(struct numval));

  if (!a){
    yyerror("out of space");
    exit(0);
  }

  a->nodetype = 'K';
  a->number = d;
  return (struct ast *)a;
}

int eval(struct ast *a){
  int v = 0; /* result value */

  switch(a->nodetype){
  case 'K': v = ((struct numval *)a)->number; break;

  case '+': v = eval(a->l) + eval(a->r); break;
  case '-': v = eval(a->l) - eval(a->r); break;
  case '*': v = eval(a->l) * eval(a->r); break;
  case DIV: v = eval(a->l) / eval(a->r); break;
  case '%': v = eval(a->l) % eval(a->r); break;
  case 'M': v = - eval(a->l); break;
  default: printf("internal error: bad node %c\n", a->nodetype);
  }
  return v;
}

void treefree(struct ast *a){
  /* note there is no break before K, so a '+' will free r first and then l */
  switch (a->nodetype){
  /* 2 subtrees */
  case '+':
  case '-':
  case '*':
  case DIV:
  case '%': treefree(a->r);

  /* 1 subtree */
  case 'M': treefree(a->l);

  /* 0 subtrees */
  case 'K': free(a); break;

  default: printf("internal error: free bad node %c\n", a->nodetype);
  }
}

void yyerror(char *s, ...){
  va_list ap;
  va_start(ap, s);

  fprintf(stderr, "%d: error : ", yylineno);
  vfprintf(stderr, s, ap);
  fprintf(stderr, "\n");
}

int main(){
  #if YYDEBUG
  yydebug = 1;
  #endif

  printf("> ");
  return yyparse();
}
