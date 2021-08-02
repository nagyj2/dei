/** Testing suite for the symbols.c structures and methods.
 * This file serves to provide a definition to the @p symbol_suite variable declared in tests.h.
 * @file test_symbol.c
 * @author Jason Nagy (jaysun_n@hotmail.com)
 * @version 0.1
 * @date 2021-07-26
 * 
 * @copyright Copyright (c) 2021
 * 
 */

#include <string.h>

#include "tests.h"
#include "symbols.h"

#define LENGTH 5

/* Test general usage of Symbol */
TEST testSymbol_Usage(void) {
	char *name[LENGTH] = {"var1", "_var2", "var_3", "VAR4", "VAR_5"};
	int i;

	for (i = 0; i < LENGTH; i++) {
		Symbol *sym = lookup(name[i]);

		ASSERT_EQ(0, strcmp(name[i], sym->name));	/* Assert proper name */
		ASSERT_NEQ(name[i], sym->name);						/* Assert unaliased */
		ASSERT_EQ('I', sym->func->nodetype);			/* Assert default */

		Symbol *alias = lookup(name[i]);
		ASSERT_EQ(sym, alias);

		freeSymbol(&sym);
		ASSERT_EQ(NULL, sym);
		alias = lookup(name[i]);
		ASSERT_EQ(NULL, alias);
	}

	PASS();
}

/* Test assignment of values */
TEST testSymbol_Assignment(void) {
	char *name = "var6";
	Symbol *sym = lookup(name);
	ASSERT_EQ(0, ((NatInt *) (sym->func))->integer);
	
	AST *def = newNatint(5);
	setsym(sym, def);
	def = lookup(name)->func;
	ASSERT_EQ(5, ((NatInt *) def)->integer);

	PASS();
}

/* Create a symbol call */
TEST testSymbol_SymbolRef(void) {
	char *name = "var7";
	Symbol *sym = lookup(name);
	AST *ref = newSymcall(sym);

	ASSERT_EQ('E', ((SymbolRef *) ref)->sym->func->nodetype);

	freeAst_Symbol(&ref);
	ASSERT_EQ(NULL, ref);
	freeSymbol(&sym);
	ASSERT_EQ(NULL, sym);

	PASS();
}

SUITE(symbol_suite) {
	RUN_TEST(testSymbol_Usage);
	RUN_TEST(testSymbol_Assignment);
	RUN_TEST(testSymbol_SymbolRef);
}
