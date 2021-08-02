/** Implementation of the symbol-less AST.
 * @file ast.c
 * @author Jason Nagy (jaysun_n@hotmail.com)
 * @version 0.1
 * @date 2021-07-26
 * 
 * @copyright Copyright (c) 2021
 * 
 */

#include <stdlib.h>
#include <stdio.h>

#ifdef DEBUG
#include <assert.h>
#endif

#include "parser.tab.h"

#include "ast.h"


/* ===== FUNCTIONS ===== */

/**
 * Allocates new memory for the node and returns the pointer.
 */
AST *newAst(int nodetype, AST *l, AST *r){
	AST *a = malloc(sizeof(AST));
	// printf("new ast @%p\n", a);

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

/**
 * Allocates new memory for the node and returns the pointer.
 * Is only used to generate comparison-type nodes.
 */
AST *newCmp(int cmptype, AST *l, AST *r){
	AST *a = malloc(sizeof(AST));
	// printf("new cmp @%p\n", a);

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

/**
 * Allocates new memory for the node and returns the pointer. 
 * Is only used to generate function-type nodes.
 */
AST *newFunc(int functype, AST *body, AST *args){
	AST *a = malloc(sizeof(AST));
	// printf("new func @%p\n", a);

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


/**
 * Allocates new memory for the node and returns the pointer. 
 * Natural dice are defined to hand a start and end face and contain all digits between.
 * If the maximum face is lower than the minimum, the result is flipped.
 */
AST *newNatdie(int count, int min, int max){
		NatDie *a = malloc(sizeof(NatDie));
		// printf("new natdie @%p\n", a);

		if (!a){
			printf("out of space\n");
			exit(1);
		}

		if (max < min) {
			int t = min;
			min = max;
			max = t;
		}

		a->nodetype = 'D';
		a->count = count;
		a->min = min;
		a->max = max;
		#ifdef DEBUG
		assert(a);
		#endif
		return (AST *)a;
}

/**
 * Allocates new memory for the node and returns the pointer. 
 * Die faces are represented as a sequence of values.
 */
AST *newSetdie(int count, ValueChain *faces){
		SetDie *a = malloc(sizeof(SetDie));
		// printf("new setdie @%p\n", a);

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
		return (AST *)a;
}

/**
 * Allocates new memory for the node and returns the pointer.
 */
AST *newNatint(int integer){
		NatInt *a = malloc(sizeof(NatInt));
		// printf("new natint @%p\n", a);

		if (!a){
			printf("out of space\n");
			exit(1);
		}

		a->nodetype = 'I';
		a->integer = integer;
		#ifdef DEBUG
		assert(a);
		#endif
		return (AST *)a;
}

/**
 * Allocates new memory for the node and returns the pointer. 
 * All parameters are stored as integers.
 * seltype should be from @ref Selectors.
 */
AST *newFargs(int fcount, int seltype, int scount, int cond){
		FuncArgs *a = malloc(sizeof(FuncArgs));
		// printf("new fargs @%p\n", a);

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
		return (AST *)a;
}

/**
 * Allocates new memory for the node and returns the pointer. 
 * A set roll is a roll which has been 'faked', but still has access to functions.
 */
AST *newSetres(ValueChain *out){
	SetRoll *a = malloc(sizeof(SetRoll));
	// printf("new setres @%p\n", a);

	if (!a){
		printf("out of space\n");
		exit(1);
	}

	a->nodetype = 'Q';
	a->out = out;
	#ifdef DEBUG
	assert(a);
	#endif
	return (AST *)a;
}


/* ===== MEMORY MANAGEMENT ===== */

/**
 * Recursively frees allocated memory in an @ref AST tree.
 * Also sets each pointer to NULL as it executes.
 */
void freeAst( AST **root ){

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
	case 'D': case 'I': case 'C':
		break;

		/* special - setdie */
	case 'd':
		freeValue( &((SetDie *)*root)->faces );
		break;

		/* special - setres */
	case 'Q':
		freeValue( &((SetRoll *)*root)->out );
		break;

	default:
		printf("unknown ast free, got %d\n", (*root)->nodetype);
		*root = NULL;
		return;

	}
	free((*root));
	*root = NULL;
}


/* ===== DEBUGGING ===== */

/**
 * Prints an entire @ref AST to standard output.
 */
void printAst(AST *root){
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
		printf("%dd[%d..%d]", ((NatDie *)root)->count, ((NatDie *)root)->min, ((NatDie *)root)->max);
		break;

	case 'I':
		printf("%d", ((NatInt *)root)->integer);
		break;

	case 'C':
		printf("$%d,%d,%d,%d$", ((FuncArgs *)root)->fcount, ((FuncArgs *)root)->seltype, ((FuncArgs *)root)->scount, (((FuncArgs *)root)->cond) ? ((FuncArgs *)root)->cond : -1);
		break;

		/* special - setdie */
	case 'd':
		printf("%dd[", ((SetDie *)root)->count);
		printValue(((SetDie *)root)->faces);
		printf("]");
		break;

		/* special - setres */
	case 'Q':
		printf("{");
		printValue(((SetRoll *)root)->out);
		printf("}");
		break;

	default:
		printf("\nunknown ast print, got %d\n", root->nodetype);

	}
}
