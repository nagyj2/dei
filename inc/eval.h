/**
 * @file eval.h
 */

#ifndef EVAL_H_INCLUDED
#define EVAL_H_INCLUDED


#include "struct.h"


/** @depreciated A representation of a die.
 */
struct diedef {
	struct value *faces;	/**< All faces on the die */
};

/** @depreciated A representation of a die roll with faces.
 */
struct rollres {
	struct value *faces;	/**< The faces on the die. */
	struct value *out;		/**< The result numbers from the rolling. */
};

/** @depreciated A representation of a die roll without die information.
 */
struct output {
	struct value *out;		/**< The result numbers from rolling. */
};

/** The result type used for evaluation.
 * The result type used type to determine which other field will be used for tracking the result.
 */
struct result {
	int type;							/**< The type of result. Determines which field will be populated. Depends on production being evaluated. Can be D for die def, R for roll result, r for roll without faces and I for integer. */
	struct value *faces;	/**< If type is D or R, this represents the faces on the die. Otherwise it is NULL.  */
	struct value *out;		/**< If type is R or r, this represents the actual rolled numbers. Otherwise it is NULL. */
	int *integer;					/**< If type is D, this represents the number of times to roll the die. If type is I, it represents an integer. Otherwise it is NULL. */
};


#endif /* EVAL_H_INCLUDED */
