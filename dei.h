/* dei.h
 * Declarations for the dei prgram
 */

/*
#define YYDEBUG
*/


/* interface to the lexer */
extern int yylineno;        /* current lexer lin */
extern int yydebug;         /* debug flag */
extern FILE *yyin;          /* input stream */
void yyerror(char *s, ...); /* similar to printf */
int yyparse(void);

/* --------------- STRUCTURES --------------- */

/* ====== DATA ====== */

/* Support Data
 * function type                (bifs)
 * selector type                (sifs)
 * eval result type             (rifs)
 *
 * chainable values             (value)
 * symbol table reference       (sym)
 * eval result hybrid container (sym)
 * eval die results             ((TODO))
 */

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

enum rifs {             /* result type */
  R_int = 1,            /* an integer result */
  R_roll,               /* a set of rolls and the die used */
  R_die                 /* a die with faces and times to roll */
};

struct value {          /* a linked list of values */
  int v;                /* value */
  struct value *next;   /* next element in list */
};

struct symbol {         /* a user defined symbol */
  char *name;           /* identifier */
  struct ast *func;     /* meaning of symbol -> an ast */
};

struct die {            /* result for a die defintion */
  int count;            /* number of rolls */
  struct value *faces;  /* die faces */
};

struct roll {           /* result for a die roll */
  struct value *out;    /* roll result */
  struct value *faces;  /* used die - WARNING : NULLABLE */
};

struct result {         /* result value of */
  int type;             /* type of result -> rifs */
  int i;                /* holder for integer */
  struct roll *r;       /* holder for roll result, die used */
  struct die *d;        /* holder for die faces, rolls required */
};


/* ==== AST NODE ==== */

/* Node Operations
 * CMP + - * DIV % ^
 * & | && ||
 * M
 *
 * Node Types
 * OP ast     : operator for arbitrary binops and unops
 * R  ast     : operatior to roll natural die -> act like Q
 * r  ast     : operatior to roll special die -> act like Q
 * S  ast     : operatior to sum rolled dice -> act like I
 * D  natdie  : definition of a natural die
 * d  setdie  : definition of a special face die
 * Q  setres  : a set roll outcome set
 * F  funcall : call to a special function
 * I  natint  : individual integer
 * E  symcall : a call to the symbol table
 * A  symasgn : set a symbol to an ast
 *
 * Support Nodes (not AST nodes themselves, but are used in defining some)
 * V  value   : chainable values
 */

struct ast {          /* R, r, S: the base ast node */
  int nodetype;       /* OPs R r S */
  struct ast *l;      /* left/first operand (or onlu operand) */
  struct ast *r;      /* right/second operand -> nullable */
};

struct natdie {       /* D: a die defined by max and min */
  int nodetype;       /* D */
  int count;          /* number of rolls */
  int min;            /* minimum die face */
  int max;            /* maximum die face */
};

struct setdie {       /* d: a die defined by face values */
  int nodetype;       /* d */
  int count;          /* number of rolls */
  struct value *faces;/* faces of the die */
};

struct natint {       /* I: natural integer */
  int nodetype;       /* I */
  int integer;        /* integer value */
};

struct setres {       /* Q: a set roll outcome */
  int nodetype;       /* Q */
  struct value *faces;/* roll results */
};

struct funcall {      /* F: function call */
  int nodetype;       /* F */
  int times;          /* times to perform function */
  int functype;       /* function id */
  int selector;       /* selector id */
  struct ast *l;      /* astroll used */
};

struct symcall {      /* E: symbol call (in expression) */
  int nodetype;       /* E */
  struct symbol *s;   /* referenced symbol */
};

struct symasgn {      /* A: assign an ast to a symbol  */
  int nodetype;       /* A */
  struct symbol *s;   /* referenced symbol */
  struct ast *l;      /* the ast to assign to symbol */
};


/* -------------- SYMBOL TABLE -------------- */

#define NHASH 9997 /* size of symbol table */
struct symbol symtab[NHASH];    /* symbol table itself */
struct symbol *lookup(char *s); /* looks up a string in the symbol table and returns the entry */
void printsymtab(); /* print entries in the table */

/* ---------------- FUNCTION ---------------- */

/* build an ast */
struct ast *newast(int nodetype, struct ast *l, struct ast *r); /* op node */
struct ast *newcmp(int cmptype, struct ast *l, struct ast *r);  /* cmp op node */

struct ast *newnatdie(int count, int min, int max);             /* create a new natural die */
struct ast *newsetdie(int count, struct value *faces);          /* create a new special die */

struct ast *newsetres(struct value *faces);                     /* create a new roll definition */
                                                                /* function call */
struct ast *newfunc(int functype, int selectype, int times, struct ast *operand);
struct ast *newnatint(int integer);                             /* create an integer */

struct ast *newref(struct symbol *s);                           /* symbol call */
struct ast *newasgn(struct symbol *s, struct ast *meaning);     /* symbol assignment */

struct value *newvalue(int i, struct value *val);               /* create a value or chain them together */

/* set the value of a symbol - save the ast */
void setsym(struct symbol *name, struct ast *val);

/* evaluate ast */
struct result *eval(struct ast *a);

/* print the ast to the screen */
void printtree(struct ast *a);

/* print a chain of values */
void printvalue(struct value *a);


/* delete and free ast */
void treefree(struct ast *a);

/* free all elements of a value chain */
void valuefree(struct value *a);

/* free result variable */
void resultfree(struct result *a);

/* perform a function on a function call */
struct result *callbuiltin(struct result *output, int functype, int selectype, struct ast *frame);
