#include "greatest.h"

#include "struct.h"

/* A test case */
TEST testValueAssign(void){
  struct value *v = NULL;
  int t = 4;

  v = newValue(t, NULL);
  ASSERT_EQ(t, v->i);
  ASSERT_EQ(NULL, v->next);
  PASS();
}

/* Create test suite to contain like cases */
SUITE(value_assignment_suite){
  RUN_TEST(testValueAssign);
}

/* Add definitions that need to be in the test runner's main file */
GREATEST_MAIN_DEFS();

int main(int argc, char **argv){
  /* Initialization */
  GREATEST_MAIN_BEGIN();

  /* Run individual tests */
  /* RUN_TEST(testValueAssign); */

  /* Run all tests in the suite */
  RUN_SUITE(value_assignment_suite);

  /* Display results */
  GREATEST_MAIN_END();

  return 0;
}