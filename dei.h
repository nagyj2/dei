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
extern FILE *yyin;          /* input stream */
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
  /* int is achieved by substituting the sifs input with a positive number */
};

enum rtype {            /* result type */
  R_int = 1,
  R_die
};

struct value {          /* a linked list of values */
  int v;                /* value */
  struct value *next;   /* next element in list */
};

struct die {            /* describe an individual die or (in eval) a rolled die */
  int size;             /* length of faces */
  int max;              /* maximum integer possible on the die : NULL if die is set */
  int min;              /* minimum integer possible : NULL if die is set */
  struct value *faces;  /* possible faces on die or (in eval) rolled numbers */
};

struct symbol {         /* a user defined symbol */
  char *name;           /* identifier */
  struct ast *func;     /* meaning of symbol -> a roll ast */
};

struct result {
  int type;             /* type of result -> rtype */
  int ivalue;           /* the integer value if type=R_int */
  struct die *rvalue;   /* the roll value if type=R_roll */
};

/* ==== AST NODE ==== */

/* Node Types
* + - * DIV %                    (ast)
* < > <= >= == !=                (ast)
*
* M unary minus                  (ast)
* Q sum rolls                    (ast)
* F built in function call       (fncall)
* D die roll result              (dieroll)
* S set die rolls                (dieroll)
* I integer                      (integer)
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
  int count;          /* how many times to perform operation */
  struct ast *l;      /* ast to execute function on */
};

struct dieroll {      /* die roll leaf node */
  int nodetype;       /* type 'D' or 'S' */
  int count;          /* amount of rolls */
  struct die *used;   /* the die used to get the result */
  /*struct value *rolls;*//* roll outcomes -> dont need to be stored in the AST */
};

struct integer{       /* integer leaf node */
  int nodetype;       /* type 'I' */
  int value;          /* integer value */
};

struct symref {       /* symbol reference */
  int nodetype;       /* type 'N' */
  struct symbol *s;   /* reference to symbol in symtab */
};

struct symasgn {      /* symbol assignment */
  int nodetype;       /* type 'A' */
  struct symbol *s;   /* ident */
  struct ast *l;      /* value */
};

/* -------------- SYMBOL TABLE -------------- */

struct symbol symtab[NHASH];    /* symbol table itself */
struct symbol *lookup(char *s); /* looks up a string in the symbol table and returns the entry */

/* ---------------- FUNCTION ---------------- */

/* build an ast */
struct ast *newast(int nodetype, struct ast *l, struct ast *r); /* op node */
struct ast *newcmp(int cmptype, struct ast *l, struct ast *r);  /* cmp op node */
                                                                /* function call */
struct ast *newfunc(enum bifs functype, enum sifs selector, struct ast *base, int count);
struct ast *newref(struct symbol *s);                           /* symbol call */
struct ast *newasgn(struct symbol *s, struct ast *meaning);     /* symbol assignment */
struct ast *newint(int value);                                  /* leaf integer */
struct ast *newroll(int count, struct die *dice);               /* perform a die roll */
struct ast *setroll(struct die *rolls);                         /* set the rolls to the face */
struct ast *newsum(struct ast *roll);                           /* sum rolls into a single int */

struct die *newdie(int min, int max);                           /* create a die from min and max */
struct die *setdie(struct value *rolls);                        /* create a die from a list of values */

struct value *addvalue(int i, struct value *val);               /* create a value or chain them together */

/* define a function - saves the parsed ast */
/* void dodef(struct symbol *name, struct symlist *syms, struct ast *stmts); */
void setsym(struct symbol *name, struct ast *val);

/* evaluate ast */
struct result *eval(struct ast *a);

/* delete and free ast */
void treefree(struct ast *a);
void diefree(struct die *a);

/* ------------- AST EVALUATION ------------- */

#ifdef FIX

/* sum a set of rolls */
int sum(struct dieroll *a);

/* evaluate a roll to a single set of dicerolls */
int eval(struct ast *a);

/* perform a function on a function call */
/* static */ int callbuiltin(struct fncall *);

#endif
