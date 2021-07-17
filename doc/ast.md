@page Abstract Syntax Tree

### Description

During parsing, an AST is maintained for future evaluation. Each node can have a type and zero to
two children. The node type will determine how many and what type of children the node will have.
For more information, see @ref inc/ast.h "ash.h".

#### Structure

The possible AST node types are as follows:
* ast: Contains up to 2 children, the left and right.
		* '+': Add two integer children together and return the integer result.
		* '-': Subtract right integer child from left interg child and return the integer result.
		* '*': Multiply two integer children together and return the integer result.
		* DIV: Divide left integer child by right integer child and return the integer result.
		* '%': Modulo left integer child by right integer child and return the integer result.
		* '^': Raise the left integer child to the power of the right integer child and return the integer result.
		* 'M': Perform unary minus on the left integer child and return the integer result.
		* '1': If left integer child is greater than the left integer child, return 1. Otherwise return 0.
		* '2': If left integer child is less than the left integer child, return 1. Otherwise return 0.
		* '3': If left integer child is not equal to the left integer child, return 1. Otherwise return 0.
		* '4': If left integer child is equal to the left integer child, return 1. Otherwise return 0.
		* '5': If left integer child is greater than or equal to the left integer child, return 1. Otherwise return 0.
		* '6': If left integer child is less than or equal to the left integer child, return 1. Otherwise return 0.
		* 'e': Append elements selected by the right child to the result of the left child.
		* 'f': Remove elements selected by the right child from the left child.
		* 'g': Count elements selected by the right child from the left child.
		* 'h': Return only elements selected by the right child from the left child.
		* 'i': Reroll the values of dice elements selected by the right child from the left child.
		* 'R': Roll the die defined by the dice child.
		* 'r': Set the roll result as if the contained numbers were rolled.
		* 'S': Sum the values of a rolled die child.
		* 'Z': Remove information from a roll result, like die faces, when the information is no longer needed.
* astAsgn: Contains up to 2 children, the symbol and ast.
		* 'A': Assign the symbol child the value of the ast child.
* natdie: Contains no children.
		* 'D': A naturally defined die containing faces 1 to a maximum.
* setdie: Contains no children.
		* 'd': An artificial die containing faces which are explicitly given.
* natint: Contains no children.
		* 'I': An integer literal.
* fargs: Contains no children.
		* 'C': Function arguments. Describes how many times a funciton will occur and what elements to select and operate on.
* setres: Contains no children.
		* 'Q': An artificial roll result. Acts as if the contained elements were the roll of a die.
* symcall: Contains 1 child, a symbol.
		* 'E': A reference to a symbol stored in the symbol table.

 <!-- * ast:			+ - * DIV % ^ & | INTER UNION
 * 					M
 * 	(ineq)	1 2 3 4 5 6
 * 	(func)	e f g h i
 * 					R - roll a die
 * 					r - fake a roll
 * 					S - sum a die roll
 * 					Z - strip information about dice (set faces = NULL)
 * astAsgn:	A - assign a value to a symbol
 *
 * Leafs
 * natdie:	D - A natural die
 * setdie:	d - An artificial die
 * natint:	I - A number literal
 * fargs:		C - selector
 * setres:	Q - An artificial roll
 * symcall:	E - a symbol reference
 */ -->
