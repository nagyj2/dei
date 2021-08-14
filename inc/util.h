/** Utility functions for Dei.
 * @file util.h
 * @author Jason Nagy (jaysun_n@hotmail.com)
 * @version 0.1
 * @date 2021-07-31
 * 
 * @copyright Copyright (c) 2021
 * 
 */

#ifndef UTIL_H_INCLUDED
#define UTIL_H_INCLUDED

extern int gSilent;

/** Produce a random integer.
 * Number is between @p min and @p max, inclusive.
 * @param[in] min Minimum number to generate.
 * @param[in] max Maximum number to generate.
 * @return int A random integer.
 */
int randint(int min, int max);


#endif /* UTIL_H_INCLUDED */
