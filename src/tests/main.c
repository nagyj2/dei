/** The primary unit test runner for Dei testing.
 * @file main.c
 * @author Jason Nagy (jaysun_n@hotmail.com)
 * @version 0.1
 * @date 2021-07-26
 * 
 * @copyright Copyright (c) 2021
 * 
 */

#include "tests.h"

/* Add definitions that need to be in the test runner's main file */
GREATEST_MAIN_DEFS();

int main(int argc, char **argv){
  /* Initialization */
  GREATEST_MAIN_BEGIN();

  /* Run individual tests */
  /* RUN_TEST(testValueAssign); */

  /* Run all tests in the suite */
  RUN_SUITE(value_suite);
  RUN_SUITE(ast_suite);
  RUN_SUITE(symbol_suite);

  /* Display results */
  GREATEST_MAIN_END();
  return 0;
}