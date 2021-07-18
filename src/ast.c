/**
 * @file ast.c
 */

#include <stdlib.h>
#include <stdio.h>

#include "parser.tab.h"

#include "ast.h"


/** Creates a new AST node.
 * Allocates new memory for the node and returns the pointer. If there is no space,
 * a message is shown and an crash occurs. Cannot return NULL.
 */
struct ast *newAst(int nodetype, struct ast *l, struct ast *r){
	struct ast *a = malloc(sizeof(struct ast));

	if (!a){
		printf("out of space\n");
		exit(1);
	}

	a->nodetype = nodetype;
	a->l = l;
	a->r = r;
	#ifdef DEBUG
	assert(a);
	#endif
	return a;
}

/** Creates a new comparison AST node.
 * Allocates new memory for the node and returns the pointer. If there is no space,
 * a message is shown and an crash occurs. Cannot return NULL.
 */
struct ast *newCmp(int cmptype, struct ast *l, struct ast *r){
	struct ast *a = malloc(sizeof(struct ast));

	if (!a){
		printf("out of space\n");
		exit(1);
	}

	a->nodetype = '0' + cmptype;
	a->l = l;
	a->r = r;
	#ifdef DEBUG
	assert(a);
	#endif
	return a;
}

/** Creates a new function AST node.
 * Allocates new memory for the node and returns the pointer. If there is no space,
 * a message is shown and an crash occurs. Cannot return NULL.
 */
struct ast *newFunc(int functype, struct ast *body, struct ast *args){
	struct ast *a = malloc(sizeof(struct ast));

	if (!a){
		printf("out of space\n");
		exit(1);
	}

	a->nodetype = 'd' + functype;
	a->l = body;
	a->r = args;
	#ifdef DEBUG
	assert(a);
	#endif
	return a;
}

/** Creates a new assignment node.
 * Allocates new memory for the node and returns the pointer. If there is no space,
 * a message is shown and an crash occurs. Cannot return NULL.
 */
struct ast *newAsgn(struct symbol *sym, struct ast *def){
	struct astAsgn *a = malloc(sizeof(struct astAsgn));

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

/** Allocates memory for a new die node and fills it in.
 * If memory cannot be allocated, a crash occurs. Cannot be NULL.
 */
struct ast *newNatdie(int count, int min, int max){
		struct natdie *a = malloc(sizeof(struct natdie));

		if (!a){
			printf("out of space\n");
			exit(1);
		}

		a->nodetype = 'D';
		a->count = count;
		a->min = min;
		a->max = max;
		#ifdef DEBUG
		assert(a);
		#endif
		return (struct ast *)a;
}

/** Allocates memory for a artificial die node and fills it in.
 * If memory cannot be allocated, a crash occurs. Cannot be NULL.
 */
struct ast *newSetdie(int count, struct value *faces){
		struct setdie *a = malloc(sizeof(struct setdie));

		if (!a){
			printf("out of space\n");
			exit(1);
		}

		a->nodetype = 'd';
		a->count = count;
		a->faces = faces;
		#ifdef DEBUG
		assert(a);
		#endif
		return (struct ast *)a;
}

/** Allocates memory for a new integer node and fills it in.
 * If memory cannot be allocated, a crash occurs. Cannot be NULL.
 */
struct ast *newNatint(int integer){
		struct natint *a = malloc(sizeof(struct natint));

		if (!a){
			printf("out of space\n");
			exit(1);
		}

		a->nodetype = 'I';
		a->integer = integer;
		#ifdef DEBUG
		assert(a);
		#endif
		return (struct ast *)a;
}

/** Allocates memory for function arguments.
 * If memory cannot be allocated, a crash occurs. Cannot be NULL.
 */
struct ast *newFargs(int fcount, int seltype, int scount, int cond){
		struct fargs *a = malloc(sizeof(struct fargs));

		if (!a){
			printf("out of space\n");
			exit(1);
		}

		a->nodetype = 'C';
		a->fcount = fcount;
		a->seltype = seltype;
		a->scount = scount;
		a->cond = cond;
		#ifdef DEBUG
		assert(a);
		#endif
		return (struct ast *)a;
}

/** Allocates memory for an artificial die roll and fills it in.
 * If memory cannot be allocated, a crash occurs. Cannot be NULL.
 */
struct ast *newSetres(struct value *out){
	struct setres *a = malloc(sizeof(struct setres));

	if (!a){
		printf("out of space\n");
		exit(1);
	}

	a->nodetype = 'Q';
	a->out = out;
	#ifdef DEBUG
	assert(a);
	#endif
	return (struct ast *)a;
}

/** Allocates memory for a new symbol call and fills it in.
 * If memory cannot be allocated, a crash occurs. Cannot be NULL.
 */
struct ast *newSymcall(struct symbol *sym){
	struct symcall *a = malloc(sizeof(struct symcall));

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


/** Assigns a definition to a symbol.
 * All symbol references contain the function definition of the variable because they are
 * all pointers to the same location, so the definition pointer is simply freed if present
 * and then re-set.
 */
void setsym(struct symbol *name, struct ast *def){
	//DEBUG_REPORT("place at %p", name);
  if (name->func){ /* NOTE allocated in lookup, so this will always run*/
    freeAst( &(name->func) );
    name->func = malloc(sizeof(struct ast));
  }

  name->func = def;
}


/* === MEMORY MANAGEMENT === */

/** Free the memory allocated to an AST tree.
 * Recursively frees allocated memory according to DFS.
 * Also sets each pointer to NULL as it completes.
 */
void freeAst( struct ast **root ){

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

	free(*root);
	*root = NULL;
}


/* ======= DEBUGGING ======= */

/** Outputs an entire AST tree.
 * Logs an entire AST tree to stdout.
 */
void printAst(struct ast *root){
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
		printf("(st.");
		printAst(root->l);
		printf(", ");
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
