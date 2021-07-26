/** Implemention details of symbols.h.
 * @file symbols.c
 * @author Jason Nagy (jaysun_n@hotmail.com)
 * @version 0.1
 * @date 2021-07-26
 * 
 * @copyright Copyright (c) 2021
 * 
 */

#include <stdlib.h>
#include <stdio.h>

#include "parser.tab.h"

#include "symbols.h"


/* === DATA === */

struct symbol symtab[NHASH];    /**< Definition of the symbol table. */


/* === FUNCTIONS === */

/** Perform a hash algorithm to determine positioning in the symbol table.
 * @param  sym The symbol to hash upon.
 * @return     The index to start looking in the symbol table.
 */
static unsigned symhash(char *sym){

  unsigned int hash = 0;
  unsigned c;

  while ((c = *sym++)) hash = hash*9 ^ c;
  return hash;
}

/**
 * Initial position is determined by hashing it and then iteratively looking for either
 * an existing entry or a free space top place it. If the entry already exists, it will always
 * be found before an empty slot, assuming no entries are deleted.
 * Causes a crash if the symbol table is filled.
 */
struct symbol *lookup(char *sym){

  struct symbol *sp = &symtab[symhash(sym)%NHASH]; /* symtab position's contents */
  int scount = NHASH; /* how many slots we have yet to look at */

  while (--scount >= 0){
    /* if entry exists, check if it is the same and return if it is */
    if (sp->name && !strcmp(sp->name, sym)) {

      //DEBUG_REPORT("found %s at %p\n", sp->name, sp);
      // printf("found %s at %p\n",sp->name,sp);
      return sp;
    }

    /* if no entry, make an entry and return it */
    if (!sp->name){
      sp->name = strdup(sym);
			sp->func = newNatint(0); /* init to zero */
      //DEBUG_REPORT("new %s at %p(%p)\n", sp->name, sp, sp->func);
			// printf("new %s at %p\n",sp->name,sp);
      return sp;
    }

    /* if no return, try the next entry */
    /* increment the pointer and if out of bounds (goto next), loop to start of symtab */
    if (++sp >= symtab+NHASH) sp = symtab;
  }

  printf("symbol table overflow\n");
  exit(2); /* tried all entries, table is full */
}


/**
 * Allocates memory for a new symbol call and fills it in.
 * If memory cannot be allocated, a crash occurs. Cannot be NULL.
 */
struct ast *newSymcall(struct symbol *sym){
	struct symcall *a = malloc(sizeof(struct symcall));
	// printf("new symcall @%p\n", a);

	if (!a){
		printf("out of space\n");
		exit(1);
	}

	a->nodetype = 'E';
	a->sym = sym;
	#ifdef DEBUG
	assert(a);
	#endif
	return (struct ast *)a;
}

/**
 * Allocates new memory for the node and returns the pointer. If there is no space,
 * a message is shown and an crash occurs. Cannot return NULL.
 */
struct ast *newAsgn(struct symbol *sym, struct ast *def){
	struct astAsgn *a = malloc(sizeof(struct astAsgn));
	// printf("new asgn @%p\n", a);

	if (!a){
		printf("out of space\n");
		exit(1);
	}

	a->nodetype = 'A';
	a->s = sym;
	a->l = def;
	#ifdef DEBUG
	assert(a);
	#endif
	return (struct ast *)a;
}


/**
 * All symbol references contain the function definition of the variable because they are
 * all pointers to the same location, so the definition pointer is simply freed if present
 * and then re-set.
 */
void setsym(struct symbol *name, struct ast *val){
	freeAst( &(name->func) );
		#ifdef DEBUG
	assert(name->func == NULL);
		#endif
	name->func = val;
}


/* === MEMORY MANAGEMENT === */

/**
 * 
 * @param[] sym 
 */
void freeSymbol(struct symbol **sym){
	if ((*sym)->func)
		freeAst(&(*sym)->func);
	free((*sym)->name);
	*sym = NULL;
}

/**
 * 
 */
void freeTable(){
	int i = 0;
	for (i = 0; i < NHASH; i++){
		struct symbol *sp = &symtab[i];
		if (sp->name)
		{
			// printf("found %s at index %d\n", sp->name, i);
			freeSymbol(&sp);
		}
	}
}

/**
 * Recursively frees allocated memory according to DFS.
 * Also sets each pointer to NULL as it completes.
 */
void freeAst_Symbol( struct ast **root ){

	switch ((*root)->nodetype){
		/* 2 subtrees */
	case '+': case '-': case '*': case DIV: case '%': case '^':
	case '&': case '|': case INTER: case UNION:
	case '1': case '2': case '3': case '4': case '5': case '6':
	case 'e': case 'f': case 'g': case 'h': case 'i':
		freeAst( &(*root)->r );

		/* 1 subtree */
	case 'M': case 'R': case 'S': case 'Z':
		freeAst( &(*root)->l );

		/* 0 subtrees */
	case 'D': case 'I': case 'C': case 'E':
		break;

		/* special - setdie */
	case 'd':
		freeValue( &((struct setdie *)*root)->faces );
		break;

		/* special - setres */
	case 'Q':
		freeValue( &((struct setres *)*root)->out );
		break;

		/* special - astAsgn */
	case 'A':
		freeAst( &((struct astAsgn *)*root)->l );
		break;

	default:
		printf("unknown ast free, got %d\n", (*root)->nodetype);
		*root = NULL;
		return;

	}
	free((*root));
	*root = NULL;
}


/* === DEBUGGING === */

/**
 * Logs an entire AST tree to stdout.
 */
void printAst_Symbol(struct ast *root){
	switch (root->nodetype){
		/* 2 subtrees */
	case '+': case '-': case '*': case '%': case '^':
	case '&': case '|':
		printf("(");
		printAst(root->l);
		printf("%c",root->nodetype);
		printAst(root->r);
		printf(")");
		break;

	case INTER: case UNION: case DIV:
		printf("(");
		printAst(root->l);
		switch (root->nodetype){
		case INTER: printf("&&"); break;
		case UNION: printf("||"); break;
		case DIV: printf("//"); break;
		}
		printAst(root->r);
		printf(")");
		break;

	case '1': case '2': case '3': case '4': case '5': case '6':
		printf("(");
		printAst(root->l);
		switch (root->nodetype){
		case '1': printf(">"); break;
		case '2': printf("<"); break;
		case '3': printf("!="); break;
		case '4': printf("=="); break;
		case '5': printf(">="); break;
		case '6': printf("<="); break;
		}
		printAst(root->r);
		printf(")");
		break;

	case 'e': case 'f': case 'g': case 'h': case 'i':
		switch (root->nodetype){
		case 'e': printf("Append"); break;
		case 'f': printf("Drop"); break;
		case 'g': printf("Count"); break;
		case 'h': printf("Choose"); break;
		case 'i': printf("Reroll"); break;
		}
		printf("(");
		printAst(root->l);
		printf(" st.");
		printAst(root->r);
		printf(")");
		break;

		/* 1 subtree */
	case 'M': case 'R': case 'S': case 'Z':
		printf("%c(",root->nodetype);
		printAst(root->l);
		printf(")");
		break;

		/* 0 subtrees */
	case 'D':
		printf("%dd[%d..%d]", ((struct natdie *)root)->count, ((struct natdie *)root)->min, ((struct natdie *)root)->max);
		break;

	case 'I':
		printf("%d", ((struct natint *)root)->integer);
		break;

	case 'C':
		printf("$%d,%d,%d,%d$", ((struct fargs *)root)->fcount, ((struct fargs *)root)->seltype, ((struct fargs *)root)->scount, (((struct fargs *)root)->cond) ? ((struct fargs *)root)->cond : -1);
		break;

	case 'E':
		printf("%s", ((struct symcall *)root)->sym->name );
		break;

		/* special - setdie */
	case 'd':
		printf("%dd[", ((struct setdie *)root)->count);
		printValue(((struct setdie *)root)->faces);
		printf("]");
		break;

		/* special - setres */
	case 'Q':
		printf("{");
		printValue(((struct setres *)root)->out);
		printf("}");
		break;

		/* special - astAsgn */
	case 'A':
		printf("%s := ", ((struct astAsgn *)root)->s->name);
		printAst(((struct astAsgn *)root)->l);
		break;

	default:
		printf("\nunknown ast print, got %d\n", root->nodetype);

	}
}