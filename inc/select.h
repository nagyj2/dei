/** Contains @ref SelectionChain structure, enabling selection of multiple disconnected @ref ValueChain's.
 * Also contains declarations for functions to manipulate these structures.
 * @file select.h
 * @author Jason Nagy (jaysun_n@hotmail.com)
 * @version 0.1
 * @date 2021-07-26
 * 
 * @copyright Copyright (c) 2021
 * 
 */

#ifndef SELECT_H_INCLUDED
#define SELECT_H_INCLUDED


#include <stdbool.h> /* bool */

#include "value.h"


/** A structure for tracking a disconnected set of values.
 * Used in evaluation algorithms, such as when an operation needs to occur to multiple, not neccessarily connected elements.
 * TODO: Change #val from ValueChain * to ValueChain **
 */
struct value_selection {
	ValueChain *val;			        /**< A value being 'selected', or points to other result node's values. Cannot be NULL. */
	struct value_selection *next;	/**< The next selected element. Can be NULL. */
};

/** Shorthand for the value_selection structure. */
typedef struct value_selection SelectionChain;



/* ===== FUNCTIONS ===== */

/** Creates a @ref SelectionChain node which points to a @ref ValueChain pointer.
 * If @p prev is not NULL, the new selection will point its next field to @p prev, continuing the chain.
 * If @p prev is NULL, an entirely new selection chain will be created.
 * @param[in] val  The @ref ValueChain pointer which is being 'selected'.
 * @param[in] prev The value the new selection's next should point to.
 * @return    A pointer to the new @ref SelectionChain head.
 */
SelectionChain *newSelection(ValueChain *val, SelectionChain *prev);

/** Returns the back of a series of @ref SelectionChain's.
 * If @p base is NULL, NULL is returned.
 * @param[in] base The head of a selection chain.
 * @return    The back of the input @ref SelectionChain.
 */
SelectionChain *backSelection(SelectionChain *base);

/** Return the @ref SelectionChain which has a pointed to @ref ValueChain with a specific integer value.
 * Goes through the elements of the chain and looks for an element which has a specific integer value.
 * If @p key doesnt share a value with any pointed to @ref ValueChain's, NULL is returned.
 * @param[in] key  The integer key to search for.
 * @param[in] base The selection to search in.
 * @return    The selection element which has the searched for key.
 */
SelectionChain *findSelection(int key, SelectionChain *base);

/** Search a @ref SelectionChain for an exact @ref ValueChain instance and return it if present.
 * Goes through the elements of the chain and looks for an element which has a specific memory address.
 * If @p key doesnt share an address with any pointed to @ref ValueChain's, NULL is returned.
 * @param[in] key  The memory address of the struct being searched for.
 * @param[in] base The selection to search in.
 * @return    The selection element which has the same address as the key.
 */
SelectionChain *findSelectionExact(ValueChain *key, SelectionChain *base);

/** Remove first instance of an integer value from a selected element in a @ref SelectionChain sequence.
 * Looks for a specific value in a chain elements and removes it from the chain if found.
 * Returns NULL if @p key cannot be found or @p base is NULL.
 * @param[in]     key  The integer value to look for.
 * @param[in,out] base The address of the selection to search in.
 * @return        If @p key is found, the @ref SelectionChain which used to be in @p base.
 */
SelectionChain *removeSelection(int key, SelectionChain **base);

/** Removes a specific instance of a @ref ValueChain from a @ref SelectionChain sequence.
 * Looks for a specific memory address in a chain elements and removes it from the chain if found.
 * Returns NULL if @p key cannot be found or @p base is NULL.
 * @param[in]     key  The memory address of a @ref ValueChain to look for.
 * @param[in,out] base The address of the selection to search in.
 * @return        If @p key is found, the @ref SelectionChain which used to be in @p base.
 */
SelectionChain *removeSelectionValue(ValueChain *key, SelectionChain **base);

/** Removes a specific instance of a @ref SelectionChain in a sequence.
 * Looks for a specific memory address and removes it from the chain if found.
 * Returns NULL if @p key cannot be found or @p base is NULL.
 * @param[in]     key  The memory address of a @ref SelectionChain to look for.
 * @param[in,out] base The address of the selection to search in.
 * @return        If @p key is found, the @ref SelectionChain which used to be in @p base.
 */
SelectionChain *removeSelectionExact(SelectionChain *key, SelectionChain **base);


/** Copies a single @ref SelectionChain, aliasing the contained data.
 * A utility function to relicate a single entity.
 * The created entity's 'selected' value is aliased.
 * @deprecated Undesirable action. It is preferred to not alias @ref SelectionChain's because it becomes much more unmanageable.
 * @param[in]	base The structure element to copy.
 * @return    A new @ref SelectionChain copy of @p base which aliases its value.
 */
SelectionChain *copySelection(SelectionChain *base);


/** Counts the number of elements in the input @ref SelectionChain and returns it.
 * If @p base is NULL, 0 is returned.
 * @param[in] base The selection to count the size of. Can be NULL.
 * @return    An integer representing the length of the selection.
 */
int countSelection(SelectionChain *base);

/** Searches a @ref SelectionChain to determine if a another is contained within.
 * False is returned if @p base is NULL.
 * @param[in] key  A selection entity to search for.
 * @param[in] base The chain to search through.
 * @return    True if @p key is within @p base. False otherwise.
 */
bool hasSelectionExact(SelectionChain *key, SelectionChain *base);

/** Searches a @ref SelectionChain for a @ref ValueChain and returns whether it is found.
 * False is returned if @p base is NULL.
 * @param[in] key  The value to search for.
 * @param[in] base The chain to search through.
 * @return    True if @p key is within the values of a @p base element. False otherwise.
 */
bool hasSelection(ValueChain *key, SelectionChain *base);

/** Searches a @ref SelectionChain for a specific integer being contained in a value.
 * False is returned if @p base is NULL.
 * @param[in] key  An integer to search for.
 * @param[in] base The chain to search through.
 * @return    True if @p key is within a selected @ref ValueChain's value. False otherwise.
 */
bool hasSelectionInt(int key, SelectionChain *base);



/* ===== MEMORY MANAGEMENT ===== */

/** Frees memory allocated to @ref SelectionChain structures in a chain.
 * Since selectiona are aliased, this function only frees the selection nodes.
 * The selected values are untouched.
 * All freed pointers are set to NULL.
 * @param[in,out] sel The chain to free.
 * @sideeffect Input @p sel and all those subsequently pointed to will be freed and the pointers set to NULL. Also, any aliased @ref SelectionChain's will be silently set to NULL.
 */
void freeSelectionAliased(SelectionChain **sel);

/** Frees memory allocated to @ref SelectionChain structures and any pointed to @ref ValueChain elements.
 * Only the pointed to @ref ValueChain's are freed, not the entire chain they represent.
 * All freed pointers are set to NULL.
 * @param[in,out] sel The chain and its elements to free.
 * @sideeffect Input @p sel, all the aliased @ref SelectionChain elements and aliased @ref ValueChain elements to will be freed and the pointers silently set to NULL.
 */
void freeSelectionComplete(SelectionChain **sel);

/** Frees memory allocated to a selection chain.
 * Since selection values are aliased, this function only frees the selection nodes, not their values.
 * @deprecated
 * @param[in,out] sel The node to free.
 * @sideeffect The input's memory location and all those subsequently pointed to will be freed and the pointers set to NULL.
 */
void freeSelection(SelectionChain **sel);



/* ===== DEBUGGING ===== */

/** Print a @ref SelectionChain and its values to standard output.
 * @param[in] sel The chain to display.
 * @sideeffect @p sel and its contents will be displayed on standard output.
 */
void printSelection(SelectionChain *sel);

#endif /* SELECT_H_INCLUDED */