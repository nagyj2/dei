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

/* -------------- SYMBOL TABLE -------------- */
/* symbol table */
struct symbol {
  char *name;
  int value;            /* current value */
  struct ast *func;     /* stmt for the function */
  struct symlist *syms; /* list of dummy args */
};

/* simple symtab of fixed size */
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
/* ------------------------------------------ */

/* ---------- Abstract Symbol Tree ---------- */

/* Node Types
 * + - * // %
 * < > <= >= == !=
 * M unary minus
 * L expression or statement list
 * I if statement
 * W while statement
 * N symbol reference
 * = assignment
 * S list of symbols
 * F built in function call
 * C user function call
 *
 * K number constant
 */

enum bifs {   /* built-in functions */
  B_sqrt = 1,
  B_exp,
  B_log,
  B_print
};

/* nodes in the AST */
/* all have common initial nodetype */
struct ast {
  int nodetype;
  struct ast *l;
  struct ast *r;
};

struct fncall {       /* built-in function */
  int nodetype;       /* type F */
  struct ast *l;
  enum bifs functype;
};

struct ufncall {      /* user function */
  int nodetype;       /* type C */
  struct ast *l;      /* list of arguments */
  struct symbol *s;   /* symbol which holds function */
};

struct flow {         /* flow statements (if, while) */
  int nodetype;       /* I or W */
  struct ast *cond;   /* condition */
  struct ast *tl;     /* then branch or do list */
  struct ast *el;     /* optional else branch */
};

struct numval {       /* constant */
  int nodetype;       /* type K */
  int number;
};

struct symref {       /* symbol reference */
  int nodetype;       /* type N */
  struct symbol *s;
};

struct symasgn {      /* symbol assignment */
  int nodetype;       /* type = */
  struct symbol *s;   /* ident */
  struct ast *v;      /* value */
};

/* build an ast */
struct ast *newast(int nodetype, struct ast *l, struct ast *r);
struct ast *newcmp(int cmptype, struct ast *l, struct ast *r);
struct ast *newfunc(int functype, struct ast *l);
struct ast *newcall(struct symbol *s, struct ast *l);
struct ast *newref(struct symbol *s);
struct ast *newasgn(struct symbol *s, struct ast *l);
struct ast *newnum(int d);
struct ast *newflow(int nodetype, struct ast *cond, struct ast *tl, struct ast *el);

/* define a function - saves the parsed ast */
void dodef(struct symbol *name, struct symlist *syms, struct ast *stmts);
void dosym(struct symbol *name, struct ast *val);

/* evaluate ast */
int eval(struct ast *a);

/* delete and free ast */
void treefree(struct ast *a);
/* ------------------------------------------ */
