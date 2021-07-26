#include "greatest.h"

#include "tests.h"
#include "struct.h"

/* Add definitions that need to be in the test runner's main file */
GREATEST_MAIN_DEFS();

int main(int argc, char **argv){
  /* Initialization */
  GREATEST_MAIN_BEGIN();

  /* Run individual tests */
  /* RUN_TEST(testValueAssign); */

  /* Run all tests in the suite */
  RUN_SUITE(value_suite);

  /* Display results */
  GREATEST_MAIN_END();
  return 0;
}