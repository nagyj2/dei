
#include "struct.h"
#include "util.h"

/* inclusive random int */
int randint(int min, int max){
  return (rand() % (max + 1 - min)) + min;
}

/* pick a random face from the offered faces and the length of the list */
int randroll(struct value *faces){
  int index, len = countValue(faces), select = rand() % len;
  struct value *t = faces;
  /*printf("pos %d\n",index);*/
  for (index = select; index > 0; index--)
    t = t->next;
  return t->v;
}


/* print the ast to the screen */
void printsymtab(void){
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

/* print a chain of values */
void printvalue(struct value *val){
  struct value *t;
  printf("val chain");
  for(t = val; t != NULL; t = t->next)
    printf(" %d", t->v);
  printf("\n");
}

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
    printf("F(%d,%d,%d,(",
      ((struct funcall *)a)->functype,
      ((struct funcall *)a)->sel->sel,
      ((struct funcall *)a)->sel->count
    );
    printtree(((struct funcall *)a)->l);
    printf(")x%d)", ((struct funcall *)a)->times);
    break;

  case 'I':
    printf("%d",((struct natint *)a)->integer);
    break;

  case 'E':
    printf("$%s:", ((struct symcall *)a)->s->name );
    printtree( ((struct symcall *)a)->s->func );
    printf("$");
    break;

  case 'A':
    printf("%s", ((struct symasgn *)a)->s->name );
    printf(" ::= ");
    printtree( ((struct symasgn *)a)->l );
    break;

  }
}
