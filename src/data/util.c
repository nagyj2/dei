/** Implementation of utility functions.
 * @file util.c
 * @author Jason Nagy (jaysun_n@hotmail.com)
 * @version 0.1
 * @date 2021-07-31
 * 
 * @copyright Copyright (c) 2021
 * 
 */

#include <stdlib.h>

#include "util.h"

/**
 * To use, the random number generator must be seeded.
 */
int randint(int min, int max) {
	return (rand() % (max + 1 - min)) + min;
}
