
#include <stdlib.h> /* malloc, exit */
#include <stdio.h> /* printf */
#include <string.h> /* strdup */

#include <assert.h> /* assert */

#include "struct.h"

/* ===== DATAS ===== */

struct symbol symtab[NHASH];    /* symbol table itself */

/* === FUNCTIONS === */

/* create new single value or append a chain */
struct value *newValue(int i, struct value *prev){
	struct value *a = malloc(sizeof(struct value));
	if (!a){
		printf("out of space");
		return NULL;
	}

	a->i = i;
	a->next = prev;
	#ifdef DEBUG
	assert(a && a->i == i && a->next == prev);
	#endif
	return a;
}

/* copy a value chain */
struct value *dupValue(struct value *base){
	struct value *head = NULL, *t = NULL;

	for(t = base; t; t = t->next){
		head = newValue(t->i, head);
	}

	reverseValue( &head );

	#ifdef DEBUG
	struct value *h = NULL;
	for(t = base, h = head; t || h; t = t->next, h = h->next){
		assert(t->i == h->i);
	}
	#endif

	return head;
}

/* return the back of a chain */
struct value *backValue(struct value *base){
	struct value *t = NULL;

	for(t = base; t->next; t = t->next){
	}

	#ifdef DEBUG
	assert(t && t->i && !t->next);
	#endif
	return t;
}

/* return the first struct containing key  */
struct value *findValue(int key, struct value *base){
	struct value *t = NULL;

	for(t = base; t; t = t->next){
		if (t->i == key) return t;
	}
	#ifdef DEBUG
	assert(!hasValue(key, base));
	#endif
	return NULL;
}

/* remove key IF it is in base and return it */
struct value *popValue(int key, struct value **base){
	struct value *t = NULL, *prev = NULL;

	#ifdef DEBUG
	int size = countValue(*base);
	#endif

	for(t = *base; t; t = t->next){
		if (t->i == key){
			if (prev)	prev->next 	= t->next; /* skip over current */
			else			*base 			= t->next; /* set front to next */

			t->next = NULL; /* erase link */
			#ifdef DEBUG
			assert(!hasValueExact(t, *base));
			assert(size - 1 == countValue(*base));
			#endif
			return t;
		}

		prev = t;
	}

	#ifdef DEBUG
	assert(!hasValue(key, *base));
	assert(size == countValue(*base));
	#endif
	return NULL;
}

/* remove key IF it is in base and return it */
struct value *removeValue(struct value *key, struct value **base){
	struct value *t = NULL, *prev = NULL;

	#ifdef DEBUG
	int size = countValue(*base);
	assert(countValue(key) == 1);
	#endif

	for(t = *base; t; t = t->next){
		if (t == key){
			if (prev)	prev->next 	= t->next; /* skip over current */
			else			*base 			= t->next; /* set front to next */

			t->next = NULL; /* erase link */
			#ifdef DEBUG
			assert(!hasValueExact(t, *base));
			assert(size - 1 == countValue(*base));
			#endif
			return t;
		}

		prev = t;
	}

	#ifdef DEBUG
	assert(!hasValueExact(key, *base));
	assert(size == countValue(*base));
	#endif
	return NULL;
}

/* invert the order of a value chain */
void reverseValue(struct value **base){
	struct value *prev = NULL, *curr = *base, *next = NULL;

	while(curr){
		next = curr->next;
		curr->next = prev;
		prev = curr;
		curr = next;
	}
	*base = prev;

}


/* copy a single value */
struct value *copyValue(struct value *base){
	return newValue(base->i, NULL);
}

/* create a value chain from min and max */
struct value *newValueChain(int min, int max){
	if (min > max) printf("warning: min > max. empty value\n");
	struct value *val1 = NULL;
	for (int i = max; i >= min; i--)
		val1 = newValue(i, val1);
	return val1;
}


/* return number of elements in base */
int countValue(struct value *base){
	struct value *t = NULL;
	int c = 0;

	for(t = base; t; t = t->next)	c++;

	#ifdef DEBUG
	assert(c >= 0);
	#endif
	return c;
}

/* return sum elements of base  */
int sumValue(struct value *base){
		struct value *t = NULL;
		int s = 0;

		#ifdef DEBUG
		int size = countValue(base);
		#endif

		for(t = base; t; t = t->next)	s += t->i;

		#ifdef DEBUG
		assert(size == countValue(base));
		#endif
		return s;
}

/* return whether exact value is in base */
bool hasValueExact(struct value *key, struct value *base){
		struct value *t = NULL;

		#ifdef DEBUG
		//assert(countValue(key) == 1); /* disallow chains */
		#endif

		for(t = base; t; t = t->next){
			if (t == key){
				return true;
			}
		}

		return false;
	}

/* return whether int value is in base */
bool hasValue(int key, struct value *base){
	struct value *t = NULL;

	for(t = base; t; t = t->next){
		if (t->i == key) return true;
	}

	return false;
}


/* create a new symbol table */
/*void newSymtab(struct symbol *result[]){
}*/

/* hash a symbol to produce an index to place in symbol table at */
static unsigned symhash(char *sym){

  unsigned int hash = 0;
  unsigned c;

  while ((c = *sym++)) hash = hash*9 ^ c;
  return hash;
}

/* return spot in symtab */
struct symbol *lookup(char *sym){

  struct symbol *sp = &symtab[symhash(sym)%NHASH]; /* symtab position's contents */
  int scount = NHASH; /* how many slots we have yet to look at */

  while (--scount >= 0){
    /* if entry exists, check if it is the same and return if it is */
    if (sp->name && !strcmp(sp->name, sym)) {

      //DEBUG_REPORT("found %s at %p\n", sp->name, sp);
      #ifdef DEBUG
      printf("found %s at %p\n",sp->name,sp);
			#endif
      return sp;
    }

    /* if no entry, make an entry and return it */
    if (!sp->name){
      sp->name = strdup(sym);
      //DEBUG_REPORT("new %s at %p(%p)\n", sp->name, sp, sp->func);
			#ifdef DEBUG
			printf("new %s at %p\n",sp->name,sp);
			#endif
      return sp;
    }

    /* if no return, try the next entry */
    /* increment the pointer and if out of bounds (goto next), loop to start of symtab */
    if (++sp >= symtab+NHASH) sp = symtab;
  }

  printf("symbol table overflow\n");
  exit(2); /* tried all entries, table is full */
}


/* === MEMORY MANAGEMENT === */

/* free a value chain */
void freeValue( struct value **val ){
	struct value *nval = NULL;
  while(*val){
    nval = (*val)->next;
    free(*val);
    *val = NULL;
    *val = nval;
  }

	assert(!*val);
}


/* ======= DEBUGGING ======= */

/* iteratively print a value chain */
void printValue(struct value *base){
	struct value *t;
	for (t = base; t; t = t->next)
		printf("%d ",t->i);
	//printf("\n");
}
