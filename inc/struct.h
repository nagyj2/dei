/** Contains @ref ValueChain structure, enabling chain-able integers.
 * Also contains declarations for functions which operate on them.
 * @file struct.h
 * @author Jason Nagy (jaysun_n@hotmail.com)
 * @version 0.1
 * @date 2021-07-26
 * 
 * @copyright Copyright (c) 2021
 * 
 */

#ifndef STRUCT_H_INCLUDED
#define STRUCT_H_INCLUDED

#include <stdbool.h> /* bool */


/* ===== DATA ===== */

/** A structure to contain an integer and a link to another value.
 * Used to represent multiple data structures, such as die rolls and faces.
 */
struct value {
	int i;								/**< The value stored at the current node. */
	struct value *next;		/**< The next element in the chain. Can be NULL. */
};

/** Shorthand for the value structure. */
typedef struct value ValueChain;



/* ===== FUNCTIONS ===== */

/** Create either a new ValueChain or append a new element.
 * If @p prev is NULL, a structure will be created. Otherwise, a new structure will be created with @p prev as the next element.
 * @param[in]	i    Integer value to assign to the new structure.
 * @param[in] prev Previous head structure. Can be NULL.
 * @return    A new @ref ValueChain structure with non-NULL value field.
 */
ValueChain *newValue(int i, ValueChain *prev);

/** Duplicates an input ValueChain.
 * Goes through the input chain to create a @ref ValueChain containing the same values.
 * The new elements are entirely separate from the input; they are not aliased.
 * @param[in]	base The head element of the value chain to duplicate. The duplication will be from this element to the end.
 * @return    The head of the newly created @ref ValueChain.
 */
ValueChain *dupValue(ValueChain *base);

/** Find the last element in a @ref ValueChain and return it.
 * If @p base is NULL, NULL is returned.
 * @param[in]	base The chain to find the last value of.
 * @return    The last @ref ValueChain element in @p base.
 */
ValueChain *backValue(ValueChain *base);

/** Return the @ref ValueChain which contains a specifc integer value.
 * Goes through the elements of the chain and looks for an element which has a specific value.
 * If @p base does not contain @p key, NULL is returned.
 * @param[in]	key  The integer value to search for.
 * @param[in]	base The chain of values to search in.
 * @return    The first instance of a value element holding @p key.
 */
ValueChain *findValue(int key, ValueChain *base);

/** Remove first instance of an integer value from a @ref ValueChain and return it.
 * Looks for a specific value of the chain elements and removes it from the chain if found.
 * Returns NULL if @p key cannot be found or @p base is NULL.
 * @param[in]  		key  The integer value to search for.
 * @param[in,out]	base The address of a pointer to search through.
 * @return      If @p key is found, the element containing @p key is returned.
 * @sideeffect	@p base will no longer contain the returned @ref ValueChain.
 */
ValueChain *removeValue(int key, ValueChain **base);

/** Remove an exact @ref ValueChain in another and remove it if present.
 * Compares memory locations of the inputs to identify a match and subsequent removal.
 * Returns NULL if @p key cannot be found or @p base is NULL.
 * @param[in]  		key	A pointer to the value to remove.
 * @param[in,out] base The address of a pointer to search through.
 * @return      If @p key is found, the element containing @p key is returned.
 * @sideeffect	@p base will no longer contain the returned @ref ValueChain.
 */
ValueChain *removeValueExact(ValueChain *key, ValueChain **base);

/** Reverse the order of a @ref ValueChain.
 * @param[in,out] base The address of a pointer to reverse.
 * @sideeffect 		@p base will be reversed. Any pointers aliased will not be updated.
 */
void reverseValue(ValueChain **base);


/** Copies a single @ref ValueChain.
 * A utility function to duplicate a single element of @p base.
 * The created entity is separate from the input and is not aliased.
 * @param[in]	base The structure element to copy.
 * @return    A new @ref ValueChain copy of @p base.
 */
ValueChain *copyValue(ValueChain *base);

/** Creates a new @ref ValueChain and populates it with sequential numbers.
 * A utility function to create a brand new chain.
 * This method creates from @p min to @p max inclusive.
 * @param[in]	min The lower end of the new chain.
 * @param[in]	max The higher end of the new chain. Must be greater than or equal to @p min.
 * @return   	A pointer to a new @ref ValueChain which contains elements [ @p min, @p max ].
 */
ValueChain *newValueChain(int min, int max);


/** Counts the number of elements in the input @ref ValueChain and returns it.
 * If @p base is NULL, 0 is returned.
 * @param[in]	base The chain to count the elements of.
 * @return    Returns the integer number of elements in base.
 */
int countValue(ValueChain *base);

/** Sum all the values contained in a series of @ref ValueChain.
 * If @p base is NULL, 0 is returned.
 * @param[in]	base The chain to sum the values of.
 * @return    Returns the integer sum of elements in base.
 */
int sumValue(ValueChain *base);

/** Checks to see whether an exact @ref ValueChain is in another chain.
 * False is returned if @p base is NULL.
 * @param[in]	key  The value to search for. Can be a value chain, but only current element will be searched for.
 * @param[in]	base The chain to search through.
 * @return    True if @p key is within @p base. Otherwise false.
 */
bool hasValueExact(ValueChain *key, ValueChain *base);

/** Checks to see whether an integer value is within a @ref ValueChain.
 * False is returned if @p base is NULL.
 * @param[in]	key  The integer value to search for.
 * @param[in]	base The chain to search through.
 * @return    True if @p key is contained is within an element of @p base. Otherwise false.
 */
bool hasValue(int key, ValueChain *base);



/* ===== MEMORY MANAGEMENT ===== */

/** Frees allocated memory from a series of @ref ValueChain.
 * @param[in,out]	val The address to a chain which will be freed.
 * @sideeffect 		All pointers to released data will equal to NULL. This will impact aliased references.
 */
void freeValue(ValueChain **val);



/* ===== DEBUGGING ===== */

/** Outputs a representation of a @ref ValueChain to standard output.
 * @param[in]		base The chain to output the contents of.
 * @sideeffect 	@p base's contents will be sent to standard output.
 */
void printValue(ValueChain *base);



#endif /* STRUCT_H_INCLUDED */
