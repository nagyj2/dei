/** Available test suites.
 * @file tests.h
 * @author Jason Nagy (jaysun_n@hotmail.com)
 * @version 0.1
 * @date 2021-07-25
 * 
 * @copyright Copyright (c) 2021
 * 
 */

#ifndef TESTS_H_INCLUDED
#define TESTS_H_INCLUDED

#define MAX(a,b) ((a > b) ? a : b)
#define MIN(a,b) ((a < b) ? a : b)
#define LEN(a,b) (MAX(a, b) - MIN(a, b) + 1)
#define CONSSUM(start,end) (int) (((float) LEN(start, end) / 2.0)  * (start + end))

/* Value test suite */
void value_suite(void);

#endif /* TESTS_H_INCLUDED */