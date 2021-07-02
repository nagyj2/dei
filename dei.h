/* dei.h
 * Declarations for the dei prgram
 */

/* interface to the lexer */
extern int yylineno; /* from lexer */
extern int yydebug;

void yyerror(char *s, ...); /* similar to printf */
int yyparse(void);

/* nodes in the AST */
struct ast {
  int nodetype;
  struct ast *l;
  struct ast *r;
};

struct numval {
  int nodetype; /* hold K if constant */
  int number;
};

/* build ast */
struct ast *newast(int nodetype, struct ast *l, struct ast *r);
struct ast *newnum(int d);

/* evaluate ast */
int eval(struct ast *root);

/* delete and free ast */
void treefree(struct ast *root);
