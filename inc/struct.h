/** Basic structures which support Dei.
 * Contains common elements to all other modules of Dei along with functions to work with the structs.
 * @file struct.h
 */

#ifndef STRUCT_H_INCLUDED
#define STRUCT_H_INCLUDED

#define NHASH 9997 /**< Size of the symbol table. */

#include <stdbool.h> /* bool */


/* === DATA === */

/** An chain of integer values.
 * Stores an integer value and a link to the next one in the chain.
 */
struct value {
	int i;												/**< The value stored at the current node. */
	struct value *next;						/**< The next element in the chain. Can be NULL. */
};

/** Symbol reference.
 * A symbol which stores an ast which is inserted into any expressions which reference the value.
 */
struct symbol {
 char *name;          					/**< The name of the symbol. */
 struct ast *func;    					/**< The meaning of the symbol. */
};

/* === FUNCTIONS === */

/* = Value = */

/** Create a new value.
 * If @ref prev is NULL, a new chain will be created. Otherwise, a new value struct will be created with @ref prev as the next element.
 * @param[in]  i    Integer value to assign to the new value element.
 * @param[in]  prev Previous head element. Can be NULL.
 * @return      A head element of a chain of value structs.
 */
struct value *newValue(int i, struct value *prev);
/** Copies an entire value chain.
 * Goes through the input chain to create a new chain containing the same values. The new elements are not aliased.
 * to the old chain; they are completely separate.
 * @param[in]  base The head element of the value chain to duplicate.
 * @return      The head of the newly created chain.
 */
struct value *dupValue(struct value *base);
/** Returns the last element in a chain.
 * @param[in]  base The chain to find the last value of.
 * @return      The last element in the input chain.
 */
struct value *backValue(struct value *base);
/** Return the first element containing a specific value.
 * Goes through the elements of the chain and looks for an element which has a specific value.
 * @param[in]  key  The element value to look for.
 * @param[in]  base The value chain to search in.
 * @return      Returns the first instance of a value element holding key or NULL if the key was not found.
 */
struct value *findValue(int key, struct value *base);
/** Remove first instance of value key and return it.
 * Looks for a specific value of the chain elements and removed it from the chain if found.
 * Returns NULL if key cannot be found or *base is NULL.
 * @param[in]  key  The integer value to search for.
 * @param[in,out]  base The address of a chain pointer to search through.
 * @return      If key is found, the removed element is returned. Otherwise NULL.
 * @sideeffect	base will no longer contain the return value in its chain if it is non-NULL.
 */
struct value *removeValue(int key, struct value **base);
/** Remove an exact value struct instance from a chain and return it.
 * Looks for a specific value pointer within the input chain and removes and returns it if found.
 * Returns NULL if key cannot be found, key is NULL or *base is NULL.
 * @param[in]  key  A pointer to the value to remove.
 * @param[in,out]  base The address of a chain pointer to search through.
 * @return      If key is found, the element is returned. Otherwise NULL.
 * @sideeffect	base will no longer contain the return value in its chain if it is non-NULL.
 */
struct value *removeValueExact(struct value *key, struct value **base);
/** Reverse the order of a value chain.
 * @param[in,out] base The address of a chain pointer to reverse.
 */
void reverseValue(struct value **base);

/** Copies a single value from a value chain.
 * A utility function to return a copy of the value of base. The two struct instances are separate and are not aliased.
 * @param[in]  base The exact value struct to copy.
 * @return      A copy of the input.
 */
struct value *copyValue(struct value *base);
/** Creates a new value chain from a start and end point.
 * A utility function to create a brand new value chain from integer min to integer max, inclusive.
 * @param[in]  min The lower end of the new chain.
 * @param[in]  max The higher end of the new chain. Must be greater than or equal to min.
 * @return     A value pointer to a new value chain.
 */
struct value *newValueChain(int min, int max);

/** Find the number of elements in a value chain.
 * @param[in]  base The chain to count the elements of.
 * @return      Returns the integer number of elements in base. If base is NULL, 0 is returned.
 */
int countValue(struct value *base);
/** Sum all the values contained in a value chain.
 * @param[in]  base The value chain to sum the values of.
 * @return      Returns the integer sum of elements in base. If base is NULL, 0 is returned.
 */
int sumValue(struct value *base);
/** Determine whether exact value struct is in a value chain.
 * @param[in]  key  The struct to search for.
 * @param[in]  base The value chain to search through.
 * @return      True if key is within base. Otherwise false.
 */
bool hasValueExact(struct value *key, struct value *base);
/* return whether int value is in base */
/** Determine whether a specific integer value is in a value chain.
 * @param[in]  key  The integer value to search for.
 * @param[in]  base The value chain to search through.
 * @return      True if key is within base. Otherwise false.
 */
bool hasValue(int key, struct value *base);


/* === MEMORY MANAGEMENT === */

/** Frees an entire value chain.
 * @param[in,out] val The address to a value chain which will be freed.
 * @sideeffect After the fucntion finished evaluation, all pointers to released data will equal to NULL.
 */
void freeValue( struct value **val );

/* free the entire symbol table */
//void freeTable( struct symbol *table[] );


/* ======= DEBUGGING ======= */

/* iteratively print a value chain */
/** Outputs a representation of a value chain.
 * @param[in] base The value chain to output.
 * @sideeffect base's contents will be sent to stdout.
 */
void printValue(struct value *base);

#endif /* STRUCT_H_INCLUDED */
