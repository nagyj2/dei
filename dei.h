/* dei.h
 * Declarations for the dei prgram
 */

/*
#define YYDEBUG
*/

#define NHASH 9997 /* size of symbol table */

/* interface to the lexer */
extern int yylineno;        /* from lexer */
extern int yydebug;         /* debug flag */
void yyerror(char *s, ...); /* similar to printf */
int yyparse(void);

/* --------------- STRUCTURES --------------- */

/* ====== DATA ====== */

enum bifs {             /* built-in functions */
  B_drop = 1,
  B_append,
  B_choose,
  B_reroll,
  B_count
};

enum sifs {             /* selectors */
  S_high = -1,          /* highest in dieroll */
  S_low = -2,           /* lowest in dieroll */
  S_rand = -3,          /* random roll in dieroll */
  S_unique = -4         /* all unique rolls in dieroll */
  /* S_all = -5             all rolls in dieroll */
};

struct value {          /* a linked list of values */
  int v;                /* value */
  struct value *next;   /* next element in list */
};

struct die {            /* describe an individual die */
  int size;             /* length of faces */
  struct value *faces;  /* possible faces on die */
};

struct symbol {         /* a user defined symbol */
  char *name;           /* identifier */
  struct ast *func;     /* meaning of symbol -> a roll ast */
};

/* ==== AST NODE ==== */

/* Node Types
* + - * DIV %                    (ast)
* < > <= >= == !=                (ast)
*
* M unary minus                  (ast)
* L expression or statement list (ast)
* F built in function call       (fncall)
* D die roll result              (dieroll)
* N symbol reference             (symref)
* A symbol assignment            (symasgn)
*/

struct ast {          /* the base ast node */
  int nodetype;       /* L, math symbols */
  struct ast *l;
  struct ast *r;
};

struct fncall {       /* built-in function */
  int nodetype;       /* type 'F' */
  enum bifs functype; /* function type to perform */
  enum sifs seltype;  /* selection type to perform (negative) OR a die result number (positive) */
  struct ast *l;      /* ast to execute function on */
};

struct dieroll {      /* die roll */
  int nodetype;       /* type 'D' */
  int size;           /* amount of rolls */
  struct die *used;   /* the die used to get the result */
  struct value *rolls;/* roll outcomes */
};

struct symref {       /* symbol reference */
  int nodetype;       /* type 'N' */
  struct symbol *s;   /* reference to symbol in symtab */
};

struct symasgn {      /* symbol assignment */
  int nodetype;       /* type 'A' */
  struct symbol *s;   /* ident */
  struct ast *v;      /* value */
};

/* -------------- SYMBOL TABLE -------------- */

struct symbol symtab[NHASH];    /* symbol table */
struct symbol *lookup(char *s); /* looks up a string in the symbol table and returns the entry */

#ifdef FIX

/* ---------------- FUNCTION ---------------- */

/* build an ast */
struct ast *newast(int nodetype, struct ast *l, struct ast *r); /* op node */
struct ast *newcmp(int cmptype, struct ast *l, struct ast *r);  /* cmp op node */
                                                                /* function call */
struct ast *newfunc(enum bifs functype, enum sifs selector, int count);
struct ast *newref(struct symbol *s);                           /* symbol call */
struct ast *newasgn(struct symbol *s, struct ast *l);           /* symbol assignment */

struct ast *newroll(int count, struct *die dice);               /* perform a die roll */
struct ast *setroll(struct die *rolls);                         /* set the rolls to the face */

struct die *newdie(int min, int max);
struct die *setdie(struct die* rolls);

int randint(int min, int max);
/* return (rand() % (max - min + 1)) + min; */

/* define a function - saves the parsed ast */
/* void dodef(struct symbol *name, struct symlist *syms, struct ast *stmts); */
void setsym(struct symbol *name, struct ast *val);

/* evaluate ast */
int eval(struct ast *a);

/* delete and free ast */
void treefree(struct ast *a);
void rollfree(struct die *a);
void valfree(struct value *a);

/* ------------- AST EVALUATION ------------- */

/* sum a set of rolls */
int sum(struct dieroll *a);

/* evaluate a roll to a single set of dicerolls */
int eval(struct ast *a);

/* perform a function on a function call */
/* static */ int callbuiltin(struct fncall *);

#endif
