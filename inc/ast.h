/* Dei AST Structures and Functions
 * Contents:
 * AST Structs
 * Functions to manipulate AST Structs
 */



struct leafNatdie {	/* Define a natural die */
	int nodetype;	/*  */
	int count;	/*  */
	int min;	/*  */
	int max;	/*  */
};

struct leafSetdie {	/*  */
	int nodetype;	/*  */
	int count;	/*  */
	struct value *faces;	/*  */
};

struct leafInt {	/*  */
	int nodetype;	/*  */
	int value;	/*  */
};

struct nodeAst {						/* Generic AST Node */
	int nodetype;						/*  */
	struct value *l;				/*  */
	struct value *r;				/*  */
};

struct nodeFunc {
	int nodetype;

};
