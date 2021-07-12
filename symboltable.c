
#include "symboltable.h"

struct symbol symtab[NHASH];    /* symbol table itself */


/* === FUNCTIONS === */

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

      debug_report("found %s at %p\n", sp->name, sp);
      return sp;
    }

    /* if no entry, make an entry and return it */
    if (!sp->name){
      sp->name = strdup(sym);
      sp->func = malloc(sizeof(struct ast));
      sp->func = newnatint(0);                /* initialize to protect against errors */
      debug_report("new %s at %p(%p)\n", sp->name, sp, sp->func);
      return sp;
    }

    /* if no return, try the next entry */
    /* increment the pointer and if out of bounds (goto next), loop to start of symtab */
    if (++sp >= symtab+NHASH) sp = symtab;
  }

  yyerror("symbol table overflow\n");
  abort(); /* tried all entries, table is full */
}

/* === MEMORY MANAGEMENT === */

/* free the entire symbol table */
void freesymboltable(void){
  int i;
  for (i = 0; i < NHASH; i++){
    struct symbol *sp = &symtab[i];

    if (sp->name){
      debug_report("freed %s at %p\n", sp->name, sp);
      freeAst(&sp->func);
    }
    free(sp);
  }
}
