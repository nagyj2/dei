/* dei.h
 * Declarations for the dei prgram
 */

/*
#define YYDEBUG
*/

/* interface to the lexer */
extern int yylineno;        /* from lexer */
extern int yydebug;         /* debug flag */
void yyerror(char *s, ...); /* similar to printf */
int yyparse(void);

/* --------------- STRUCTURES --------------- */

/* ====== DATA ====== */

enum bifs {   /* built-in functions */
  B_drop = 1,
  B_append,
  B_choose,
  B_reroll,
  B_count
};

enum sifs {   /* selectors */
  S_high = 1,
  S_low,
  S_rand,
  S_unique,
  S_all,
  S_int
};

struct rollset {        /* a chain of roll results */
  int roll;
  struct rollset *next;
};

struct symbol {         /* a user defined symbol */
  char *name;           /* identifier */
  struct ast *func;     /* meaning of symbol */
};

/* ==== AST NODE ==== */

/* Node Types
* + - * DIV %
* < > <= >= == !=
* M unary minus
*
* L expression or statement list
* F built in function call
* S selector
* N symbol reference
* A assignment
* K number constant
* D die roll result
*/

struct ast {          /* the base ast node */
  int nodetype;
  struct ast *l;
  struct ast *r;
};

struct fncall {       /* built-in function */
  int nodetype;       /* type 'F' */
  struct ast *l;      /* arguments */
  int count;          /* times to act */
  enum bifs functype; /* id of function to perform */
};

struct scall {        /* selector */
  int nodetype;       /* type 'S' */
  enum sifs functype; /* id of selector */
  int count;          /* number of times for int */
};

struct symref {       /* symbol reference */
  int nodetype;       /* type 'N' */
  struct symbol *s;
};

struct symasgn {      /* symbol assignment */
  int nodetype;       /* type 'A' */
  struct symbol *s;   /* ident */
  struct ast *v;      /* value */
};

struct rollresult {   /* roll result */
  int nodetype;       /* type D or type K */
  int count;          /* size of r or a constant int*/
  struct rollset *r;  /* result of rolls */
};

/* -------------- SYMBOL TABLE -------------- */
#define NHASH 9997
struct symbol symtab[NHASH];
struct symbol *lookup(char *s);

/* list of symbols , for an argument list */
struct symlist {
  struct symbol *sym;
  struct symlist *next;
};

struct symlist *newsymlist(struct symbol *sym, struct symlist *next);
void symlistfree(struct symlist *root);


/* ---------------- FUNCTION ---------------- */

/* build an ast */
struct ast *newast(int nodetype, struct ast *l, struct ast *r); /* op node */
struct ast *newcmp(int cmptype, struct ast *l, struct ast *r);  /* cmp op node */
struct ast *newfunc(int functype, struct ast *l, int count);    /* function call */
struct ast *newref(struct symbol *s);                           /* symbol call */
struct ast *newasgn(struct symbol *s, struct ast *l);           /* symbol assignment */
struct ast *newroll(int nodetype, int size);

/* define a function - saves the parsed ast */
/* void dodef(struct symbol *name, struct symlist *syms, struct ast *stmts); */
void dosym(struct symbol *name, struct ast *val);

/* evaluate ast */
int eval(struct ast *a);

/* delete and free ast */
void treefree(struct ast *a);
void rollfree(struct rollset *a);
/* ------------------------------------------ */

/* ------------- AST EVALUATION ------------- */

/* sum a set of rolls */
int sum(struct rollset *a);

/* evaluate a roll to a single set of dicerolls */
int eval(struct ast *a);

/* perform a function on a function call */
/* static */ int callbuiltin(struct fncall *);
