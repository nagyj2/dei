/** Testing suite for the select.c structures and methods.
 * All methods available to operate on selection structs are tested here in numerous cases.
 * This file serves to provide a definition to the @p select_suite variable declared in tests.h.
 * @file test_value.c
 * @author Jason Nagy (jaysun_n@hotmail.com)
 * @version 1.0
 * @date 2021-07-26
 * 
 * @copyright Copyright (c) 2021
 * 
 */

#include <stdbool.h>

#include "tests.h"
#include "value.h"
#include "select.h"

#define LENGTH 10

/* Test NULL setting of freed structures */
TEST testSelection_Free(void) {
  ValueChain *v = newValueChain(-5, LENGTH), *t = NULL;
  SelectionChain *s = NULL;

  s = newSelection(v, NULL);
  v = v->next; /* Since first element will be killed by s's free */
  /* Check simple free */
  freeSelectionAliased(&s);
  ASSERT_EQ(NULL, s);
  ASSERT_NEQ(NULL, v);
  freeValue(&v);

  /* Select entire chain */
  v = newValueChain(-5, LENGTH);
  s = NULL;
  for (t = v; t; t = t->next) {
    s = newSelection(t, s);
  }

  /* Free entire chain */
  freeSelectionComplete(&s);
  ASSERT_EQ(NULL, s);
  /* Check silent update -> CANNOT WITH CURRENT SELECTION STRUCTURE. Can set my pointer to NULL but not others */
  // ASSERT_EQ(NULL, u); /* It is freed, but hasnt been reflected in value */
  // ASSERT_EQ(NULL, v);

  PASS();
}

/* Test selection creation methods */
TEST testSelection_Creation(void) {
  ValueChain *v = newValueChain(-5, LENGTH), *t = NULL;
  SelectionChain *s = NULL, *u = NULL;

  /* Select front */
  s = newSelection(v, NULL);
  ASSERT_EQ(v, s->val);
  ASSERT_EQ(v->next, s->val->next);

  /* Alias the selected value */
  u = copySelection(s);
  ASSERT_EQ(s->val, u->val);
  ASSERT_NEQ(s, u);

  freeSelectionAliased(&s);
  freeSelectionAliased(&u);

  /* Select middle */
  t = findValue(0, v);
  s = newSelection(t, NULL);
  ASSERT_EQ(t, s->val);
  ASSERT_EQ(t->next, s->val->next);
  freeSelectionAliased(&s);

  /* Select end */
  t = findValue(LENGTH, t);
  s = newSelection(t, NULL);
  ASSERT_EQ(t, s->val);
  freeSelectionAliased(&s);

  ASSERT_EQ(NULL, s);
  for (t = v; t; t = t->next) {
    s = newSelection(t, s);
  }
  /* For loop inverts structure */
  reverseValue(&v);
  
  for (t = v, u = s; t && u; t = t->next, u = u->next) {
    ASSERT_EQ_FMT(t, u->val, "%p");
  }
  /* Assert all accounted for */
  ASSERT_EQ(NULL, u);

  freeValue(&v);
  PASS();
}

/* Test finding selections */
TEST testSelection_Find(void) {
  ValueChain *v = newValueChain(-5, LENGTH), *t = NULL;
  SelectionChain *s = NULL, *u = NULL;

  for (t = v; t; t = t->next) {
    s = newSelection(t, s);
  }
  /* For loop structure reverses order */
  reverseValue(&v);

  /* Check if back selection works */
  t = backValue(v);
  u = backSelection(s);
  ASSERT_EQ(t, u->val);
  ASSERT_EQ(NULL, u->val->next);

  /* Search for specific end element */
  u = findSelectionExact(t, u);
  ASSERT_EQ(t->next, u->val->next);
  ASSERT_EQ(t->next, u->val->next);

  /* Selecting nothing will do nothing */
  ASSERT_EQ(NULL, backSelection(NULL));

  /* Try to find specific element */
  t = findValue(0, v);
  u = findSelection(0, s);
  ASSERT_EQ(t, u->val);
  
  /* Search for specific element */
  u = findSelectionExact(t, u);
  ASSERT_EQ(t->next, u->val->next);
  ASSERT_EQ(t->next, u->val->next);

  /* Try to find front */
  t = findValue(-5, v);
  u = findSelection(-5, s);
  ASSERT_EQ(t, u->val);
  ASSERT_EQ(NULL, u->val->next);
  
  /* Search for specific front element */
  u = findSelectionExact(t, u);
  ASSERT_EQ(t->next, u->val->next);
  ASSERT_EQ(t->next, u->val->next);

  /* Try to find end */
  t = findValue(LENGTH, v);
  u = findSelection(LENGTH, s);
  ASSERT_EQ(t, u->val);

  /* Search non-existant element */
  ASSERT_EQ(NULL, findSelection(LENGTH + 2, s));
  t = newValue(LENGTH + 2, NULL);
  ASSERT_EQ(NULL, findSelectionExact(t, s));

  /* Selecting nothing will do nothing */
  ASSERT_EQ(NULL, findSelection(0, NULL));
  ASSERT_EQ(NULL, findSelectionExact(v, NULL));

  freeValue(&t);
  freeSelectionComplete(&s);

  PASS();
}

/* Test removal of elements */
TEST testSelection_Removal(void) {
  ValueChain *v = newValueChain(-5, LENGTH), *t = NULL;
  SelectionChain *s = NULL, *u = NULL;

  /* Remove from NULL */
  ASSERT_EQ(NULL, removeSelection(0, &s));
  /* Remove NULL and non-NULL ValueChain from NULL selection */
  ASSERT_EQ(NULL, removeSelectionValue(v, &s));
  ASSERT_EQ(NULL, removeSelectionValue(t, &s));
  /* Remove NULL and non-NULL SelectionChain from NULL selection */
  ASSERT_EQ(NULL, removeSelectionExact(u, &s));
  u = newSelection(v, NULL);
  ASSERT_EQ(NULL, removeSelectionExact(u, &s));
  freeSelectionAliased(&u);

  for (t = v; t; t = t->next) {
    s = newSelection(t, s);
  }

  /* Remove from start */
  t = findValue(-5, v);
  ASSERT_EQ(true, hasSelectionInt(-5, s));
  u = removeSelection(-5, &s);
  ASSERT_EQ(t, u->val);
  ASSERT_EQ(NULL, u->next);
  ASSERT_EQ(false, hasSelectionInt(-5, s));
  t = t->next; /* go to next element in selected */
  u = removeSelectionValue(t, &s);
  ASSERT_EQ(t, u->val);
  
  /* Remove from end */
  t = findValue(LENGTH-1, v);
  ASSERT_EQ(true, hasSelectionInt(LENGTH-1, s));
  u = removeSelection(LENGTH-1, &s);
  ASSERT_EQ(t, u->val);
  ASSERT_EQ(NULL, u->next);
  ASSERT_EQ(false, hasSelectionInt(LENGTH-1, s));
  t = t->next; /* go to next element in selected */
  u = removeSelectionValue(t, &s);
  ASSERT_EQ(t, u->val);

  PASS();
}

/* Test chain counting */
TEST testSelection_Count(void) {
  #define X_LEN 6
  
  ValueChain *v = NULL;
  SelectionChain *s = NULL;

  /* Count NULL */
  ASSERT_EQ(0, countSelection(s));

  /* Len 1, Negative Len 1, From zero, From 1, Negative from 0, Trans-sign */
  int min[X_LEN] = { LENGTH,  -LENGTH,  0,      1,      -LENGTH,  -LENGTH };
  int max[X_LEN] = { LENGTH,  -LENGTH,  LENGTH, LENGTH, 0,        LENGTH  };
  int c;
  for (c = 0; c < X_LEN; c++) {
    int ans = LEN(min[c], max[c]);
    v = newValueChain(min[c], max[c]);
    for (; v; v = v->next) s = newSelection(v, s);
    ASSERT_EQ(ans, countSelection(s));
    freeSelectionComplete(&s);
  }

  #undef X_LEN
  PASS();
}

/* Test value containment recognition */
TEST testSelection_Containment(void) {
  
  ValueChain *v = NULL, *t = NULL, *d = newValue(LENGTH + 1, NULL);
  SelectionChain *s = NULL;

  /* Length of NULL */
  ASSERT_EQ(false, hasSelectionInt(0, s));

  /* Check zero to positive */
  v = newValueChain(0, LENGTH);
  for (t = v; t; t = t->next) s = newSelection(t, s);
  ASSERT_EQ(true, hasSelectionInt(0, s));
  ASSERT_EQ(true, hasSelectionInt(LENGTH, s));
  ASSERT_EQ(false, hasSelectionInt(-1, s));
  ASSERT_EQ(false, hasSelectionInt(LENGTH + 1, s));
  ASSERT_EQ(true, hasSelection(v, s));
  ASSERT_EQ(true, hasSelection(backValue(v), s));
  ASSERT_EQ(false, hasSelection(d, s));
  freeSelectionComplete(&s);
  
  /* Check positive */
  v = newValueChain(1, LENGTH);
  for (t = v; t; t = t->next) s = newSelection(t, s);
  ASSERT_EQ(false, hasSelectionInt(0, s));
  ASSERT_EQ(true, hasSelectionInt(LENGTH, s));
  ASSERT_EQ(false, hasSelectionInt(-1, s));
  ASSERT_EQ(false, hasSelectionInt(LENGTH+1, s));
  ASSERT_EQ(true, hasSelection(v, s));
  ASSERT_EQ(true, hasSelection(backValue(v), s));
  ASSERT_EQ(false, hasSelection(d, s));
  freeSelectionComplete(&s);

  /* Check negative to zero */
  v = newValueChain(-LENGTH, 0);
  for (t = v; t; t = t->next) s = newSelection(t, s);
  ASSERT_EQ(true, hasSelectionInt(0, s));
  ASSERT_EQ(false, hasSelectionInt(LENGTH, s));
  ASSERT_EQ(true, hasSelectionInt(-1, s));
  ASSERT_EQ(false, hasSelectionInt(LENGTH+1, s));
  ASSERT_EQ(true, hasSelection(v, s));
  ASSERT_EQ(true, hasSelection(backValue(v), s));
  ASSERT_EQ(false, hasSelection(d, s));
  freeSelectionComplete(&s);
  
  /* Check negative */
  v = newValueChain(-LENGTH, -1);
  for (t = v; t; t = t->next) s = newSelection(t, s);
  ASSERT_EQ(false, hasSelectionInt(0, s));
  ASSERT_EQ(false, hasSelectionInt(LENGTH, s));
  ASSERT_EQ(true, hasSelectionInt(-1, s));
  ASSERT_EQ(false, hasSelectionInt(LENGTH+1, s));
  ASSERT_EQ(true, hasSelection(v, s));
  ASSERT_EQ(true, hasSelection(backValue(v), s));
  ASSERT_EQ(false, hasSelection(d, s));
  freeSelectionComplete(&s);
  
  /* Check trans-sign */
  v = newValueChain(-LENGTH, LENGTH);
  for (t = v; t; t = t->next) s = newSelection(t, s);
  ASSERT_EQ(true, hasSelectionInt(0, s));
  ASSERT_EQ(true, hasSelectionInt(LENGTH, s));
  ASSERT_EQ(true, hasSelectionInt(-1, s));
  ASSERT_EQ(false, hasSelectionInt(LENGTH+1, s));
  ASSERT_EQ(true, hasSelection(v, s));
  ASSERT_EQ(true, hasSelection(backValue(v), s));
  ASSERT_EQ(false, hasSelection(d, s));
  freeSelectionComplete(&s);

  PASS();
}

/* Create test suite to contain like cases */
SUITE(select_suite) {
  RUN_TEST(testSelection_Free);
  RUN_TEST(testSelection_Creation);
  RUN_TEST(testSelection_Find);
  RUN_TEST(testSelection_Removal);
  RUN_TEST(testSelection_Count);
  RUN_TEST(testSelection_Containment);
}
