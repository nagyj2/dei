@page Internal Structure

### Description

This page explains the internal structure and operations of the Dei program, including data types, process of evaluation, and methods.

#### Literals and Keywords

There are two main classifications of literals, integers and dice. Integers follow the form `/[0-9]+/` and dice follow the form `/<INT>*d(<INT>|\[-?<INT>..-?<INT>\]|\{-?<INT>(,-?<INT>)*\})/`. The three types of dice are standard `1` to `n` faces, `m` to `n` faces and specifically chosen die faces. Dei also has dozens of keywords which are mainly used within the function system. These keywords must be used in accordance to the grammar of Dei.

#### Mathematics

Dei contains two mathematical systems, integers and dice, which combine to create randomized math operations. All dice operations have precidence over integer operations as the dice are summed into a single 'artificial' integer for operations with the 'natural' integers. For dice literals, the dice are first rolled, any functions are applied to the result followed by set operations and then lastly they are summed into an integer, at which point they are indistinguisable from other natural integers.

#### Structures

Under the hood, Dei uses simple singly-linked lists for rolled dice and wrapped integers for natural integers. All integers are 4 byte ints, however the extremes on this data types should never be reached with reasonable Dei usage. The AST nodes are implemented through several different structures, however they all have a `nodetype` attribute in the first 4 bytes to identify its type and allow for safe conversions. Nodes can contain integers or other nodes. The nested ASTs are then evaluated recursively to produce a single output.

Alongside the ASTs, there is a symbol table which maps identifiers to ASTs. All symbols in Dei are passed by reference, so a redefinition of a symbol will affect everywhere it is references. This is normally a good thing for the usage cases envisioned by Dei, but is something to be aware of. Symbols can take any valid statement in Dei except for a roll which uses multiple dice groups. Symbols are initialized right before they are defined with the default value of 0, so typos can have unintended consequences.

#### Parsing

Dei used Flex and Bison to parse user input. This means that errors are nondescriptive and potentially difficult to find. No statements in Dei are multiline, but comments can extend through multiple line breaks. In this case, the first and last lines the comment are placed on are considered the same line. For finding errors, some common checks are:
* Typos of symbols
* Placement of operands, especially the conditional else (`;`).
* Spaces in dice literals and plural integer selectors.
