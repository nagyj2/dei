/**
 * @file eval.h
 */

#ifndef EVAL_H_INCLUDED
#define EVAL_H_INCLUDED

#include <stdlib.h>
#include <stdbool.h>
#ifdef DEBUG
#include <assert.h>
#endif

#include "struct.h"
#include "ast.h"


/** The type of resultfrom the evaluation of a specific AST node.
 * The type of node being used will imply which fields of the result are filled out.
 */
enum rifs {
	R_die = 1,
	R_roll,
	R_set,
	R_int
};

/** A structure for tracking a disconnected set of values.
 * Used for the roll element selection process.
 * Never passed between functions.
 */
struct selection {
	struct value **val;			/**< A value being 'selected', or points to other result node's values. Cannot be NULL. */
	struct selection *next;	/**< The next selected element. Can be NULL. */
};

/** The result type used for evaluation.
 * The result type used type to determine which other field will be used for tracking the result.
 * All elements, except for the val state variable of sel, are completely separate se
 */
struct result {
	int type;								/**< The type of result. Determines which field will be populated. Depends on production being evaluated. Must be from @ref rifs */
	struct value *faces;		/**< If type is @ref R_die or @ref R_roll, this represents the faces on the die. Can be NULL for R_roll. Otherwise it is NULL.  */
	struct value *out;			/**< If type is @ref R_roll or @ref R_set, this represents the actual rolled numbers. Otherwise it is NULL. */
	int *integer;						/**< If type is @ref R_int Otherwise it is NULL. */
};

#ifdef DEBUG
/** Tests a result struct to ensure relevant fields are filled and others are not.
 * @param[in] res The result struct to be tested.
 */
void ensureType(struct result *res);
#endif

/** Creates a selection node which points to the input value pointer.
 * If prev is not NULL, the new selection will point its next field to prev.
 * If prev is NULL, an entirely new selection will be created.
 * @param[in]  val  The value object to 'select'.
 * @param[in]  prev The value the new selection's next should point to. Can be NULL.
 * @return      A pointer to a new selection head.
 */
struct selection *newSelection(struct value **val, struct selection *prev);

/** Returns the back of a selection chain.
 * @param[in]  base The head of a selection chain. Can be NULL.
 * @return      The back of the selection. Can be NULL.
 */
struct selection *backSelection(struct selection *base);

/** Return the first instance of a key held within a selection.
* Returns a selection pointer to a value struct which has the same integer value as the input key.
* Can return NULL if the key is not in base.
* @param[in]  key  The integer key to search for.
* @param[in]  base The selection to search in.
* @return      The selection element which has the searched for key. Can be NULL.
*/
struct selection *findSelection(int key, struct selection *base);

/** Search for an exact value struct within a selection.
* Returns a selection pointer to a value struct which has the exact address as key.
* Can return NULL if the key is not in base.
* @param[in]  key  The value key to search for.
* @param[in]  base The selection to search in.
* @return      The selection element which has the same address as the key. Can be NULL.
*/
struct selection *findSelectionExact(struct value *key, struct selection *base);

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


#endif /* EVAL_H_INCLUDED */
