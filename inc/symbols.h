/** Extension to ast.h which deals with symbols
 * @file symbols.h
 * @author Jason Nagy (jaysun_n@hotmail.com)
 * @version 0.1
 * @date 2021-07-26
 * 
 * @copyright Copyright (c) 2021
 * 
 */

#ifndef SYMBOLS_H_INCLUDED
#define SYMBOLS_H_INCLUDED

#include "ast.h"

#define NHASH 9997 /**< Size of the symbol table. */


/* === DATA === */

/** Symbol reference.
 * A symbol which stores an ast which is inserted into any expressions which reference the value.
 */
struct symbol {
 char *name;          					/**< The name of the symbol. */
 struct ast *func;    					/**< The meaning of the symbol. */
};

/**
 * @typedef Symbol
 * Shorthand for symbol structure
 */
typedef struct symbol Symbol;


extern struct symbol symtab[NHASH];    /**< Symbol table declaration. */


/* === STRUCTURES === */

/** A symbol reference.
 * A symbol (variable) reference. The referenced symbol is guarenteed present in the symbol table, but it may not be initialized.
 */
struct symcall {
	int nodetype;						/**< E */
	struct symbol *sym;			/**< Called symbol */
};

/**
 * @typedef SymbolRef
 * Shorthand for symcall structure.
 */
typedef struct symcall SymbolRef;

/** Symbol assignment.
 * A node which will assign an ast value to a symbol. The ast value is not evaluated when saving it.
 */
struct astAsgn {
	int nodetype;						/**< A */
	struct symbol *s;				/**< Symbol to assign value to */
	struct ast *l;					/**< Value to assign to symbol */
};

/**
 * @typedef SymbolAssign
 * Shorthand for astAsgn structure.
 */
typedef struct astAsgn SymbolAssign;


/* === FUNCTIONS === */

/** Searches the symbol table for a name and returns its location.
 * If the symbol table cannot find an already created version of the symbol, it will create a new entry and return it.
 * @param[in]  s The name of the symbol to search for.
 * @return   A pointer to the storage location of the symbol within the symbol table.
 */
Symbol *lookup(char *s);
/** Create a symbol (variable) call leaf.
 * @param[in]  sym The symbol to reference. Cannot be NULL.
 * @return     An AST node representing a symbol call. Cannot be NULL.
 */
AST *newSymcall(Symbol *sym);
/** Create a new variable assignment.
 * @param[in]  sym The variable symbol to assign to.
 * @param[in]  def The AST definition of the variable.
 * @return     An AST node representing the assignment. Cannot be NULL.
 */
AST *newAsgn(Symbol *sym, AST *def);
/** Assign an AST structure to a symbol.
 * @param[in,out] name The symbol reference to assign a meaning to. Cannot be NULL.
 * @param[in] def  The definition to be assigned to name
 * @sideeffect name will have a new func value
 */
void setsym(Symbol *name, AST *def);

/* === MEMORY MANAGEMENT === */

/** Frees a symbol and the value associated with it.
 * 
 * @param[in,out] sym the symbol to be free.
 * @sideeffect @p sym is set to NULL after execution.
 */
void freeSymbol(Symbol **sym);

/** Frees the entire contents of the symbol table.
 * @sideeffect All symbols within symbol table are freed and set to NULL.
 */
void freeTable();

/** Implementation of ast.h's freeAst() which supports symbols.
 * @param[in,out] root The tree to free.
 * @sideeffect Frees memory allocated to a symbol and its saved AST.
 */
void freeAst_Symbol(AST **root);

/* create a new symbol table */
/*void newSymtab(struct symbol *result[]){
}*/


/* === DEBUGGING === */

/** Implementation of ast.h's printAst() which supports symbols.
 * @param[in] root The tree to print.
 */
void printAst_Symbol(AST *root);

#endif /* SYMBOLS_H_INCLUDED */
