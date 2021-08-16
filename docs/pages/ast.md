@page Abstract Syntax Tree

### Overview

During parsing, an AST is maintained for future evaluation. Each node can have a type and zero to
two children. The node type will determine how many and what type of children the node will have.
For more information, see @ref inc/ast.h "ash.h" and @ref inc/symbols.h "symbols.h".

#### Structure

The possible AST node types are as follows:
* @ref AST "AST": Contains up to 2 children, the left and right.
  * `+`: Add two integer children together and return the integer result.
  * `-`: Subtract right integer child from left interg child and return the integer result.
  * `*`: Multiply two integer children together and return the integer result.
  * DIV: Divide left integer child by right integer child and return the integer result.
  * `%`: Modulo left integer child by right integer child and return the integer result.
  * `^`: Raise the left integer child to the power of the right integer child and return the integer result.
  * `M`: Perform unary minus on the left integer child and return the integer result.
  * `1`: If left integer child is greater than the left integer child, return 1. Otherwise return 0.
  * `2`: If left integer child is less than the left integer child, return 1. Otherwise return 0.
  * `3`: If left integer child is not equal to the left integer child, return 1. Otherwise return 0.
  * `4`: If left integer child is equal to the left integer child, return 1. Otherwise return 0.
  * `5`: If left integer child is greater than or equal to the left integer child, return 1. Otherwise return 0.
  * `6`: If left integer child is less than or equal to the left integer child, return 1. Otherwise return 0.
  * `e`: Append elements selected by the right child to the result of the left child.
  * `f`: Remove elements selected by the right child from the left child.
  * `g`: Count elements selected by the right child from the left child.
  * `h`: Return only elements selected by the right child from the left child.
  * `i`: Reroll the values of dice elements selected by the right child from the left child.
  * `R`: Roll the die defined by the dice child.
  * `r`: Set the roll result as if the contained numbers were rolled.
  * `S`: Sum the values of a rolled die child.
  * `Z`: Remove information from a roll result, like die faces, when the information is no longer needed.
* @ref NatDie "NatDie": Contains no children.
  * `D`: A naturally defined die containing faces 1 to a maximum.
* @ref SetDie "SetDie": Contains no children.
  * `d`: An artificial die containing faces which are explicitly given.
* @ref NatInt "NatInt": Contains no children.
  * `I`: An integer literal.
* @ref FuncArgs "FuncArgs": Contains no children.
  * `C`: Function arguments. Describes how many times a function will occur and what elements to select and operate on.
* @ref SetRoll "SetRoll": Contains no children.
  * `Q`: An artificial roll result. Acts as if the contained elements were the roll of a die.
* @ref SymbolRef: Contains 1 child, a symbol.
	* `E`: A reference to a symbol stored in the symbol table.
* @ref SymbolAssign: Contains up to 2 children, the symbol and ast.
  * `A`: Assign the symbol child the value of the ast child.
* @ref IfElse: Contains 3 children.
  * `F`: A condition which will determine which of the other two children trees will be executed.
* @ref Group: Contains 2 children.
  * `G`: A container to separate multiple sequential rolls.
