/* dei STRUCTS */
/* This file contains:
 * - Struct declarations
 * - Explanation of structs
 * - essential lexer functions
 */

 /* ====== DATA ====== */

 /* Support Data
  * function type                 (bifs)
  * selector type                 (sifs)
  * eval result type              (rifs)
  *
  * chainable values              (value)
  * symbol table reference        (symbol)
  * eval rep. of a die            (die)
  * eval rep. of a die roll       (roll)
  * eval rep. of a result         (result)
  * contains a list of value ref. (selected)
  * desc. of what is selected.    (selector)
  */

#define DEI_STRUCT

enum bifs {             /* built-in functions */
 B_drop = 1,
 B_append,
 B_choose,
 B_reroll,
 B_count
};

enum sifs {             /* selectors */
  S_all = -1,
  S_high = -2,          /* highest in dieroll */
  S_low = -3,           /* lowest in dieroll */
  S_rand = -4,          /* random roll in dieroll */
  S_unique = -5         /* all unique rolls in dieroll */
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

struct selected {         /* a chain of pointers to assorted value nodes */
 struct value *val;      /* selected node */
 struct selected *next;  /* next selected node */
};

struct selector {       /* multiselect  */
 int sel;              /* what to select -> sifs */
 int count;            /* number of times to perform */
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

struct funcall {        /* F: function call */
 int nodetype;         /* F */
 int times;            /* times to perform function */
 int functype;         /* function id */
 struct selector *sel; /* selector id */
 struct ast *l;        /* astroll used */
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


/* interface to the lexer */
extern int yylineno;        /* current lexer lin */
extern int yydebug;         /* debug flag */
extern FILE *yyin;          /* input stream */
void yyerror(char *s, ...); /* similar to printf */
int yyparse(void);
