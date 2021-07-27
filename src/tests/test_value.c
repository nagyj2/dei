/** Testing suite for the struct.c structures and methods.
 * All methods available to operate on value structs are tested here in numerous cases.
 * This file serves to provide a definition to the @p value_suite variable declared in tests.h.
 * @file test_value.c
 * @author Jason Nagy (jaysun_n@hotmail.com)
 * @version 0.1
 * @date 2021-07-26
 * 
 * @copyright Copyright (c) 2021
 * 
 */

#include <stdbool.h>

#include "tests.h"
#include "struct.h"

#define LENGTH 10
#define FAILIFHAS(t,val,key) for (t = val; t; t = t->next) { if (t->i == key) FAIL(); }

/* Test value chain assignment */
TEST testValue_ChainAssign(void) {
  struct value* v = NULL;
  int i;

  for (i = 0; i < LENGTH; i++){
    v = newValue(i, v);
    ASSERT_EQ(i, v->i); /* Assert number was created */
    if (i == 0) {
      ASSERT_EQ(NULL, v->next); /* Assert first has NULL end */
    }else {
      ASSERT_NEQ(NULL, v->next); /* Assert non-first has a next */
      ASSERT_EQ(i - 1, v->next->i); /* Assert next is one less */
    }
  }

  freeValue(&v);

  PASS();
}

/* Test value duplication */
TEST testValue_Duplication(void) {
  struct value* a = NULL, * b = NULL;
  struct value *t1 = NULL, *t2 = NULL;
  int i;

  /* Duplicate NULL */
  b = dupValue(a);
  ASSERT_EQ(NULL, a);
  ASSERT_EQ(NULL, b);

  /* Duplication or a normal chain */
  for (i = 0; i < LENGTH; i++){
    a = newValue(i, a);
  }
  b = dupValue(a);

  /* Assert each element is the same */
  for (t1 = a, t2 = b; t1 && t2; t1 = t1->next, t2 = t2->next) {
    ASSERT_EQ(t1->i, t2->i);
    ASSERT_NEQ(t1, t2);
  }

  /* Assert that they ran out at the same time */
  ASSERT_EQ(NULL, t1);
  ASSERT_EQ(NULL, t2);

  freeValue(&b);

  /* Copying */
  b = copyValue(a);

  /* Assert copied element is the same */
  ASSERT_EQ(a->i, b->i);
  ASSERT_NEQ(a, b);
  ASSERT_EQ(NULL, b->next);

  freeValue(&a);
  freeValue(&b);
  
  PASS();
  }

/* Test value init equivalence */
TEST testValue_Equality(void){
  #define X_LENGTH 3
  struct value *v = NULL, *t = NULL;
  int i = 0;
  int a[X_LENGTH] = {-2, 0, 2}; /* Negative, zero, positive */

  do { /* Ensure 2 similarily initialized values are the same */
    v = newValue(a[i], NULL);
    t = newValue(a[i], NULL);
    ASSERT_EQ(v->i, t->i);
    ASSERT_EQ(v->next, t->next);
    ASSERT_NEQ(NULL, v);
    freeValue(&v);
    freeValue(&t);
  } while (++i < X_LENGTH);

  #undef X_LENGTH
  PASS();
}

/* Test freeing values */
TEST testValue_Free(void){
  struct value *v = NULL;

  /* One element */
  v = newValue(1, NULL);
  freeValue(&v);
  ASSERT_EQ(NULL, v);

  /* Multi-element */
  v = newValue(1, newValue(1, NULL));
  freeValue(&v);
  ASSERT_EQ(NULL, v);
  PASS();
}

/* Test value chain creation */
TEST testValue_ChainCreation(void) {
  struct value *v = NULL, *t = NULL;
  int i;

  /* Normal chain */
  v = newValueChain(0, LENGTH - 1);
  for (i = 0, t = v; t; i++, t = t->next)
    ASSERT_EQ(i, t->i);
  freeValue(&v);

  /* Trans-sign chain */
  v = newValueChain(-4, 5);
  for (i = -4, t = v; t; i++, t = t->next)
    ASSERT_EQ(i, t->i);
  freeValue(&v);

  /* Length 1 chain */
  v = newValueChain(4, 4);
  for (i = 4, t = v; t; i++, t = t->next)
    ASSERT_EQ(i, t->i);
  freeValue(&v);

  PASS();
}

/* Test value chain reversal */
TEST testValue_Reverse(void){
  struct value *a = NULL, *b = NULL;
  struct value *t1 = NULL, *t2 = NULL;
  int i;

  /* Reverse NULL */
  reverseValue(&a);
  ASSERT_EQ(NULL, a);

  /* Normal chain */
  /* Max -> Min */
  for (i = 0; i < LENGTH; i++)      a = newValue(i, a);

  /* Min -> Max */
  for (i = LENGTH - 1; i >= 0; i--) b = newValue(i, b);
  reverseValue(&b); /* Max -> Min */

  for (t1 = a, t2 = b; t1 && t2; t1 = t1->next, t2 = t2->next)
    ASSERT_EQ(t1->i, t2->i);

  freeValue(&a);
  freeValue(&b);

  /* Trans-sign chain */
  /* Max -> Min */
  for (i = -5; i < LENGTH; i++)       a = newValue(i, a);

  /* Min -> Max */
  for (i = LENGTH - 1; i >= -5; i--)  b = newValue(i, b);
  reverseValue(&b); /* Max -> Min */

  for (t1 = a, t2 = b; t1 && t2; t1 = t1->next, t2 = t2->next)
    ASSERT_EQ(t1->i, t2->i);

  freeValue(&a);
  freeValue(&b);

  PASS();
}

/* Test searching functions */
TEST testValue_Search(void) {
  struct value* a = NULL, * b = NULL;
  int i;

  /* Find in NULL */
  ASSERT_EQ(NULL, findValue(6, a));
  
  /* Find back of NULL */
  ASSERT_EQ(NULL, backValue(a));

  /* Max -> Min */
  for (i = 0; i < LENGTH; i++) {
    a = newValue(i, a);
    b = backValue(a); /* Gives lowest (0) */
    ASSERT_EQ(0, b->i);
    ASSERT_EQ(NULL, b->next);
  }

  /* Find in upper half */
  b = findValue(6, a);
  ASSERT_NEQ(NULL, b);
  ASSERT_EQ(6, b->i);
  ASSERT_NEQ(NULL, b->next);

  /* Find in lower half */
  b = findValue(3, a);
  ASSERT_NEQ(NULL, b);
  ASSERT_EQ(3, b->i);
  ASSERT_NEQ(NULL, b->next);

  /* Find first */
  b = findValue(LENGTH - 1, a);
  ASSERT_NEQ(NULL, b);
  ASSERT_EQ(LENGTH - 1, b->i);
  ASSERT_NEQ(NULL, b->next);

  /* Find last */
  b = findValue(0, a);
  ASSERT_NEQ(NULL, b);
  ASSERT_EQ(0, b->i);
  ASSERT_EQ(NULL, b->next);

  /* 'Find' non-existant */
  b = findValue(15, a);
  ASSERT_EQ(NULL, b);
  b = findValue(-1, a);
  ASSERT_EQ(NULL, b);

  freeValue(&a);
  freeValue(&b);
  
  PASS();
}

/* Test value removal */
TEST testValue_Removal(void) {
  struct value* v = NULL, *t = NULL, *u = NULL;
  int i;

  ASSERT_EQ(NULL, removeValue(0, &v));
  
  /* Max -> Min */
  for (i = 0; i < LENGTH; i++) {
    v = newValue(i, v);
  }

  /* Middle, Last, First */
  int a[3] = { 4, 0, 9 };
  for (i = 0; i < 3; i++) {
    t = removeValue(a[i], &v);
    ASSERT_EQ(a[i], t->i);
    ASSERT_EQ(NULL, t->next);
    FAILIFHAS(t, v, a[i]);
    freeValue(&t);
  }

  /* Missed remove */
  t = removeValue(15, &v);
  ASSERT_EQ(NULL, t);
  FAILIFHAS(t, v, 15);

  /* Middle, Last, First */
  int b[3] = { 5, 1, 8 };
  for (i = 0; i < 3; i++) {
    u = findValue(b[i], v);
    t = removeValueExact(u, &v);
    ASSERT_EQ(u, t);
    ASSERT_EQ(b[i], t->i);
    ASSERT_EQ(NULL, t->next);
    FAILIFHAS(t, v, b[i]);
    freeValue(&t);
    freeValue(&u);
  }

  /* Missed remove */
  u = newValue(15, NULL);
  t = removeValueExact(u, &v);
  ASSERT_EQ(NULL, t);
  ASSERT_NEQ(u, t); /* Both NULL */
  FAILIFHAS(t, v, 15);
  freeValue(&u);

  freeValue(&v);

  PASS();
}

/* Test chain counting */
TEST testValue_Count(void) {
  struct value* v = NULL;
  int i;
  
  /* 0 Length chain */ /* Max -> Min */
  ASSERT_EQ(0, countValue(v));

  /* Normal chain */ /* Max -> Min */
  for (i = 0; i < LENGTH; i++)
    v = newValue(i, v);
  ASSERT_EQ(LENGTH, countValue(v));
  freeValue(&v);

  /* Trans-sign chain */ /* Max -> Min */
  for (i = -5; i < LENGTH; i++)
    v = newValue(i, v);
  ASSERT_EQ(LENGTH+5, countValue(v));
  freeValue(&v);

  PASS();
}

/* Test chain counting */
TEST testValue_Sum(void) {
  struct value *v = NULL;
  int i;

  /* Zero length Sum */
  ASSERT_EQ(0, sumValue(v));
  
  /* Normal Sum */
  for (i = 0; i < LENGTH; i++)
    v = newValue(i, v);
  ASSERT_EQ_FMT(CONSSUM(1, LENGTH-1), sumValue(v), "%d");
  freeValue(&v);

  /* Trans-sign Sum */
  for (i = -5; i < LENGTH; i++)
    v = newValue(i, v);
  ASSERT_EQ(CONSSUM(-5, -1) + CONSSUM(1, LENGTH - 1), sumValue(v));
  freeValue(&v);

  /* Negative Sum */
  for (i = -5; i < 0; i++)
    v = newValue(i, v);
  ASSERT_EQ(CONSSUM(-5, -1), sumValue(v));
  freeValue(&v);
  
  /* Zero Sum */
  for (i = -LENGTH; i <= LENGTH; i++)
    v = newValue(i, v);
  ASSERT_EQ(0, sumValue(v));
  freeValue(&v);

  PASS();
}

/* Test containment */
TEST testValue_Contain(void) {
  struct value *v = NULL, *t = NULL;
  int i;

  /* Find on NULL */
  ASSERT_EQ(false, hasValue(0, v));

  /* Max -> Min */
  for (i = -LENGTH; i <= LENGTH; i++)
    v = newValue(i, v);

  /* Find first */
  ASSERT_EQ(true, hasValue(LENGTH, v));
  
  /* Find last */
  ASSERT_EQ(true, hasValue(-LENGTH, v));
  
  /* Find middle */
  ASSERT_EQ(true, hasValue(1, v));
  
  /* Find zero */
  ASSERT_EQ(true, hasValue(0, v));
  
  /* Miss find above */
  ASSERT_EQ(false, hasValue(LENGTH + 1, v));
  
  /* Miss find below */
  ASSERT_EQ(false, hasValue(-LENGTH - 1, v));

  /* Alias and find start */
  t = findValue(LENGTH, v);
  ASSERT_EQ(true, hasValueExact(t, v));
  ASSERT_EQ(true, hasValue(t->i, v));
  
  /* Alias and find end */
  t = findValue(-LENGTH, v);
  ASSERT_EQ(true, hasValueExact(t, v));
  ASSERT_EQ(true, hasValue(t->i, v));
  
  /* Alias and find middle */
  t = findValue(0, v);
  ASSERT_EQ(true, hasValueExact(t, v));
  ASSERT_EQ(true, hasValue(t->i, v));

  /* Remove element and find it */
  t = removeValue(LENGTH - 2, &v);
  ASSERT_EQ(false, hasValueExact(t, v));
  ASSERT_EQ(false, hasValue(t->i, v));

  PASS();
}



/* Create test suite to contain like cases */
SUITE(value_suite) {
  RUN_TEST(testValue_ChainAssign);
  RUN_TEST(testValue_Free);
  RUN_TEST(testValue_ChainCreation);
  RUN_TEST(testValue_Duplication);
  RUN_TEST(testValue_Equality);
  RUN_TEST(testValue_Reverse);
  RUN_TEST(testValue_Search);
  RUN_TEST(testValue_Removal);
  RUN_TEST(testValue_Count);
  RUN_TEST(testValue_Sum);
  RUN_TEST(testValue_Contain);
}
