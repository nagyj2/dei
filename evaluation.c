
#include "dei.tab.h"

#include "struct.h"
#include "evaluation.h"

#include "util.h" /* for debug printing */

/* === Data Functions === */

/* create a new selection descriptor */
/*struct selector *newSelector(int seltype, int count){
  struct selector *s = malloc(sizeof(struct selector));

  if (!s){
    printf("ran out of space");
    exit(0);
  }

  s->sel = seltype;
  s->count = count;
  return s;
}*/

/* create a chain of selected */
struct selected *newSelected(struct value *elem, struct selected *prev){
  struct selected *a = malloc(sizeof(struct selected));

  if (!a){
    yyerror("out of space");
    exit(0);
  }

  a->val = elem; /* point to the new value */
  if (prev) a->next = prev; /* if not null, set old head to the 2nd element */
  return a;
}

/* count values in a chain */
int countSelected(struct selected *sel){
  struct selected *t;
  int c = 0;

  for (t = sel; t; t=t->next) c++;
  /*printf("count %d\n",c);*/
  return c;
}

/* count values in a chain */
int sumSelected(struct selected *sel){
  struct selected *t;
  int c = 0;

  for (t = sel; t; t=t->next) c+=t->val->v;
  /*printf("count %d\n",c);*/
  return c;
}

/* search selected list for a VALUE contained within */
bool hasSelected(struct value *key, struct selected *list){
  if (!list) return false;

  struct selected *t;
  for (t = list; t; t = t->next)
    if (t->val == key)
      return true;

  return false;
}

/* search selected list for a INT contained within */
bool hasSelectedInt(int key, struct selected *list){
  if (!list) return false;

  struct selected *t;
  for (t = list; t; t = t->next)
    if (t->val->v == key)
      return true;

  return false;
}

/* places all elements missing from 'sel2' into 'sel1' */
/* after, 'sel1' is modified and 'sel2' can be freed. Returns number of transfers */
int mergeSelected(struct selected **sel1, struct selected *sel2){
  struct selected *t1 = NULL, *t2 = NULL;
  int num = 0; /* number of merges */

  if (!sel2) return num;  /* if sel2 is null, return */


  for (t2 = sel2; t2; t2 = t2->next){
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

  return num;
}

/* Returns the first element in 'opts' not in 'has' */
struct value *firstunique(struct value *opts, struct selected *has){
  struct value *p = NULL;

  for (p = opts; p; p = p->next){
      if (!hasSelected(p, has)){
        return p;
      }
  }

  return NULL;
}


/* === Dice Definitions ===*/

/* create the faces of a natural die for result */
struct value *createNatdieFace(int min, int max){
  if (min > max) yyerror("invalid die, %d,%d", min, max);
  struct value *a = NULL;
  int i = min;
  do {
    a = newValue(i++, (a) ? a : NULL); /* ensure first null pointer */
  } while (i <= max);
  return a;
}

/* create the faces of a set die for result */
struct value *createSetdieFace(struct value *faces){
  if (!faces) { yyerror("invalid set die"); return newValue(0,NULL); }
  /*printValue(a);*/
  return faces;
}

/* === Evaluation Functions === */

/* define a symbol (variable) */
void setsym(struct symbol *name, struct ast *val){
  //DEBUGLOGLN("place at %p", name);
  if (name->func){ /* NOTE allocated in lookup, so this will always run*/
    freeAst(name->func);
    name->func = malloc(sizeof(struct ast));
  }

  name->func = val;
}


/* === Memory Management === */

/* Frees a single result variable. Also frees attached data */
void freeResult(struct result *a){
  switch (a->type){
  case R_int:
    break;

  case R_roll:
    if (a->r->faces)
      freeValue(a->r->faces);
    if (a->r->out)
      freeValue(a->r->out);
    break;

  case R_die:
    if (a->d->faces)
      freeValue(a->d->faces);
    break;

  default:
    printf("unrecognized result: %c",a->type);
  }
  free(a);
}

/* Frees an entire selected chain. Does NOT free the referenced pointers */
void freeSelected(struct selected *a){
  struct selected *na;
  while(a != NULL){
    na = a->next;
    free(a);
    a = na;
  }
}

/* Recursive function to evlauate an AST */
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

    case 'I': /* create an integer */
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

      freeResult(l);
      freeResult(r);
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

      freeResult(l);
      freeResult(r);
      break;
    }

    case 'Q': /* create a artifical roll */
      v->type = R_roll;
      v->r = malloc(sizeof(struct roll));       /* need to malloc space */
      v->r->out = ((struct setres *)a)->faces;
      /* v->r->faces = NULL */
      break;

    case 'S': { /* sum a die roll */
      v->type = R_int;
      struct result *r = eval(a->l);

      if (r->type != R_roll){
        yyerror("roll node expected! got %c",r->type);
        exit(0);
      }

      v->i = sumValue(r->r->out);
      /* WARNING causes seg faults for some reason? */
      freeResult(r); /* roll result is no longer needed */
      break;
    }

    case 'D': /* create a natural die */
      v->type = R_die;
      v->d = malloc(sizeof(struct die));
      v->d->count = ((struct natdie *)a)->count;
      v->d->faces = createNatdieFace( ((struct natdie *)a)->min, ((struct natdie *)a)->max );
      /* printf("new die: "); printValue(v->d->faces); printf("\n"); */
      break;

    case 'd': /* create a artificial die */
      v->type = R_die;
      v->d = malloc(sizeof(struct die));
      v->d->count = ((struct setdie *)a)->count;
      v->d->faces = createSetdieFace(((struct setdie *)a)->faces);
      /* printf("new die: "); printValue(v->d->faces); printf("\n"); */
      break;

    case 'R': case 'r': { /* roll a die */
      v->type = R_roll;
      struct result *r = eval(a->l);

      if (r->type != R_die){
        yyerror("die node expected! got %c",r->type);
        exit(0);
      }

      v->r = malloc(sizeof(struct roll));
      v->r->faces = r->d->faces; /* duplicate pointer */
      int i = 1;
      do {
        int roll = randroll(v->r->faces);
        v->r->out = newValue(roll, (v->r->out) ? v->r->out : NULL ); /* use NULL on first */
        /* printf("roll %d\n", roll); printValue(v->r->out); */
      } while (i++ < r->d->count);

      free(r); /* free just the r pointer : keep faces */
      break;
    }

    case 'F':{ /* execute a function */
      /* all functions happen at least one time, so execute with null result value */
      v = callbuiltin( NULL, ((struct funcall *)a)->functype, ((struct funcall *)a)->seltype, ((struct funcall *)a)->scount, ((struct funcall *)a)->l );
      int i;
      for (i = 1; i < ((struct funcall *)a)->fcount; i++){
        v = callbuiltin( v, ((struct funcall *)a)->functype, ((struct funcall *)a)->seltype, ((struct funcall *)a)->scount, ((struct funcall *)a)->l );
      }
      break;
    }

    case 'E': /* symbol reference */
      v->type = R_int;
      v->i = eval( (((struct symcall *)a)->s)->func )->i;
      break;

    case 'A': /* set a symbol to a value - CURRENTLY UNUSED */
      printf("warning: untested ast node type\n");
      setsym( ((struct symasgn *)a)->s, ((struct symasgn *)a)->l );
      v->type = R_int;
      v->i = 0; //eval( ((struct symasgn *)a)->s->func );
      break;

    default:
      printf("unrecognized ast: got %c\n", a->nodetype);
  }

  assert(v);
  return v;
}

/* Compute a builtin function on a roll result */
struct result *callbuiltin(struct result *output, int functype, int seltype, int scount, struct ast *frame){
  struct result *r = NULL;
  if (output){ r = output; }
  else { r = eval(frame); }

  if (r->type != R_roll) { yyerror("expected roll type, got %d\n",r->type); return r; };


  debug_report("select type %d, %d times\n",seltype, scount);

  struct selected *sel = NULL;
  sel = select(seltype, scount, r->r); /* select appropriate values */

  #ifdef DEBUG
  debug_state("before: ");
  printValue(r->r->out);
  #endif


  /* select can return null, so verify something was selected! */
  if (sel){

    switch (functype){
      case B_drop:
      printf("warning: drop is not fully implemented\n");
      break;
      case B_append:
      printf("warning: append is not fully implemented\n");
      break;
      case B_choose:
      printf("warning: choose is not fully implemented\n");
      break;
      case B_reroll: {
        if (!r->r->faces) { yyerror("reroll requires unaltered die\n"); return r; };
        funcreroll(sel, r->r->faces);
        break;
      }

      case B_count: {
        int count = countSelected(sel);
        freeValue(r->r->out);
        r->r->out = newValue(count, NULL);
        break;
      }

      default:
        printf("unrecognized builtin id: got %d\n", functype);
    }

    #ifdef DEBUG
    debug_state("after: ");
    printValue(r->r->out);
    #endif

    freeSelected(sel);
  }


  /* since we transfer data from output to r, release output */
  //if (output) { free(output); }
  return r;
}

/* select elements of dieroll and stores them in special pointers for callbuiltin to operate on */
struct selected *select(int seltype, int scount, struct roll *dieroll){
  struct selected *retsel = NULL, *sel = NULL; /* set to null so outside it can be seen if nothing was selected */
  struct value *t; /* traversal variable */

  int i = scount;
  if (i > countValue(dieroll->out)) printf("warning: requesting more selects than available\n");
  if (i == 0) return retsel;

  do {

    switch (seltype){
      case S_high: {
        sel = newSelected(firstunique(dieroll->out, retsel),NULL); /* can't assume 1st without checking value b/c it wrecks with "sel->val->v > t->v && !hasSelected(t, retsel)" */
        if (!sel) break;
        //sel->val = dieroll->out;
        for (t = dieroll->out; t; t = t->next){ /* dont immediately go to next in case it is null */
          assert(t->v);
          if (sel->val->v < t->v && !hasSelected(t, retsel)) {  /* saved -> initialized, so never null */
            sel->val = t; /* save address */
          }
        }
        assert(sel != NULL);
        break;
      }
      case S_low: {
        sel = newSelected(firstunique(dieroll->out, retsel), NULL); /* can't assume 1st without checking value b/c it wrecks with "sel->val->v > t->v && !hasSelected(t, retsel)" */
        if (!sel) break;
        //sel->val = dieroll->out;
        for (t = dieroll->out; t; t = t->next){ /* dont immediately go to next in case it is null */
          assert(t->v);
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
        assert(seltype > 0);
        for (t = dieroll->out; t; t = t->next){
          if (t->v == seltype && !hasSelected(t, retsel)){  /* check for match */
            sel = newSelected(t, NULL);
            break; /* TODO: once per loop? */
          }
        }
        break;
    }

    if (sel){
      //if (!retsel) retsel = malloc(sizeof(struct selected));
      int merges = mergeSelected(&retsel, sel); /* merge current and newly found */
      if (sel) freeSelected(sel);
      if (merges == 0 && seltype != S_all)
        break;  /* short circuit of mergeSelected returns 0, so ensure nothing was actually entered */
    }else{
      break;
    }

  } while (--i > 0); /* stop when counting numbers, but continue */

  debug_state("selected:");
  for (sel = retsel; sel; sel = sel->next)
    debug_report(" %d",sel->val->v);
  debug_state("\n");

  if (t) free(t);
  return retsel;
}

/* rerolls all pointers pointed to by sel according to faces */
/* NEITHER sel or faces can be NULL */
void funcreroll(struct selected *sel, struct value *faces){
  /* length of faces  */
  //int facelen = countValue(faces);

  struct selected *t; /* iterator variable */
  for (t = sel; t; t = t->next){
    int roll = randroll(faces);
    t->val->v = roll; /* update pointed to's value */
  }
}
