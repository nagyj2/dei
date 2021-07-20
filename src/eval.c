/**
 * @file eval.c
 */
#include <stdio.h> /* printf */

#include "eval.h"


#ifdef DEBUG
/** Tests a result struct to ensure relevant fields are filled and others are not.
 * @param[in] res The result struct to be tested.
 */
void ensureType(struct result *res);
#endif

/**
 * Allocates memory for a new struct.
 * If memory cannot be allocated, an error is thrown.
 * If prev is NULL, a new selection chain is created.
 * Returns NULL if memory could not be allocated.
 */
struct selection *newSelection(struct value *val, struct selection *prev){
	struct selection *a = malloc(sizeof(struct selection));

	if (!a){
		printf("out of space\n");
		return NULL;
	}

	a->val = val;
	a->next = prev;
	#ifdef DEBUG
	assert(a && a->val == val && a->next = prev);
	#endif
	return a;
}

/**
 * Aliases the back element of a selection.
 */
struct selection *backSelection(struct selection *base){
	struct selection *t = NULL;
	for (t = base; t->next; t = t->next){ /* get to the end */ }

	#ifdef DEBUG
	assert(t && t->val && !t->next);
	#endif
	return t;
}

/**
 * Aliases the found element
*/
struct selection *findSelection(int key, struct selection *base){
	if (!base) return NULL;
	struct selection *t = NULL;

	for(t = base; t; t = t->next){
		if ( t->val->i == key ) return t;
	}
	#ifdef DEBUG
	assert(!hasSelection(key,base));
	#endif
	return NULL;
}

/**
 * Aliases the found element
 */
struct selection *findSelectionExact(struct value *key, struct selection *base){
	if (!base) return NULL;
	struct selection *t = NULL;

	for(t = base; t; t = t->next){
		if ( t->val == key ) return t;
	}
	#ifdef DEBUG
	assert(!hasSelectionExact(key,base));
	#endif
	return NULL;
}

/** Remove the first selection element which has a matching key and return it.
 * Can return NULL if key is not matched.
 * @param[in]  key  The integer value to look for.
 * @param[in,out]  base The selection to search in.
 * @return      The removed selection element.
 */
struct selection *removeSelection(int key, struct selection **base);

/** Remove the element which has a matching value address and return it.
 * Can return NULL if key is not matched.
 * @param[in]  key  The value struct value to look for.
 * @param[in,out]  base The selection to search in.
 * @return      The removed selection element.
 */
struct selection *removeSelectionValue(struct value *key, struct selection **base);

/** Remove the selection which matches the input key.
 * Can return NULL if key is not matched.
 * @param[in]  key  The selection to look for.
 * @param[in,out]  base The selection to search in.
 * @return      The removed selection element.
 */
struct selection *removeSelectionExact(struct selection *key, struct selection **base);


/** Duplicates a selection and its contents WITHOUT aliasing it.
 * @param[in]  base The selection to copy.
 * @return      A copied, non-aliased version of base.
 */
struct selection *copySelection(struct selection *base);


/** Counts the length of a selection.
 * If base is NULL, a length of 0 is returned.
 * @param[in]  base The selection to count the size of. Can be NULL.
 * @return      An integer representing the length of the selection.
 */
int countSelection(struct selection *base);

/** Searches a selection to determine if a selection contains another exact selection struct.
 * @param[in]  base The selection to count the size of. Can be NULL.
 * @return      True if key is within base. False otherwise.
 */
bool hasSelectionExact(struct selection *key, struct selection *base);

/** Searches a selection to determine if a selection contains an exact value struct.
 * @param[in]  base The selection to count the size of. Can be NULL.
 * @return     True if key is within base. False otherwise.
 */
bool hasSelection(struct value *key, struct selection *base);

/** Assign an AST structure to a symbol.
 * @param[in,out] name The symbol reference to assign a meaning to. Cannot be NULL.
 * @param[in] val  The definition to be assigned to name
 * @sideeffect
 */
void setsym(struct symbol *name, struct ast *val);


/** Evaluates an AST to produce an output.
 * Evaluation does not modify the input nodes, allowing for better memory management.
 * @param[in]  base The current AST node to evaluate.
 * @return      The result of the base node's operation.
 */
struct result *eval(struct ast *base);


/** Frees memory allocated to input res according to its inout type.
 * @param[in,out] res The result node to free.
 */
void freeResult(struct result **res);

/** Frees memory allocated to a selection chain.
 * Since selection values are aliased, this function only frees the selection nodes, not their values.
 * @param[in,out] res The node to free.
 */
void freeSelection(struct selection **sel);
