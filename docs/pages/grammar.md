@page Language Grammar

### Overview

Dei uses a grammar which is as close to natural language as possible to create natural sounding expressions. The starting symbol is `line`. Comments can be embedded anywhere and are delimited by `\`. There are no string or float support within Dei.

```
line:		
	|  
	| line stmt EOL
	| line EXIT EOL
	| line error EOL
	| '@'
	| line '@' stmt EOL
	| line '@' EXIT EOL
	| line '@' error EOL

stmt:
	| group
	| IDENT ':' cond
	| IDENT '=' cond

group:
	| cond
	| cond GROUP
	| cond GROUP group

cond:
	| math
	| math '?' math ';' math
	| math '?' math

math:
	| math '+' math
	| math '-' math
	| math '*' math
	| math '//' math
	| math '%' math
	| math '^' math
	| math '==' math
	| math '!=' math
	| math '>' math
	| math '<' math
	| math '>=' math
	| math '<=' math
	| '-' math
	| '(' math ')'
	| NUM
	| IDENT
	| set

set:  	
	| set '&' set
	| set '|' set
	| set '&&' set
	| set '||' set
	| '[' set ']'
	| func

func: 	
	| die
	| '{' list '}'
	| func F_ADD a_args
	| func F_SUB s_args
	| func F_MOD m_args

a_args:
	| squant ssel fquant
	| ssel fquant
	| psel fquant

s_args:
	| squant ssel
	| psel

m_args:
	| squant ssel fquant
	| squant ssel 'if' COND fquant
	| psel fquant
	| psel 'if' COND fquant

ssel:		
	| SSELECT
	| NUM

psel:
	| PSELECT
	| NUM 's'

squant:
	| NUM
	| SQUANT
	|	''

fquant:
	| NUM 'times'
	| NUM 'time'
	| FQUANT
	| ''

die: 		
	| DNUM 'd' NUM
	| DNUM 'd' '[' nnum '..' nnum ']'
	| 'd' NUM
	| 'd' '[' nnum '..' nnum ']'
	| DNUM 'd' '{' list '}'
	| 'd' '{' list '}'

list: 	
	| nnum
	| nnum ',' list

nnum:		
	| NUM
	| '-' NUM

F_ADD:		
	| 'append'

F_SUB:		
	| 'drop'
	| 'count'
	| 'choose'

F_MOD:		
	| 'reroll'

COND:			
	| 'lower'
	| 'higher'

SSELECT:
	| 'highest'
	| 'lowest'
	| 'random'

PSELECT:
	| 'unique'
	| NUM 's'

FQUANT:		
	| 'once'
	| 'twice'
	| 'thrice'

SQUANT:		
	| 'one'
	| 'two'
	| 'three'
	| 'four'
	| 'five'

EXIT:
	| 'q'
	| 'quit'

GROUP:
	| 'check'
	| 'slashing'
	| 'piercing'
	| 'bludgeoning'
	| 'poison'
	| 'acid'
	| 'fire'
	| 'cold'
	| 'radiant'
	| 'necrotic'
	| 'lightining'
	| 'thunder'
	| 'force'
	| 'psychic'

IDENT:
	| [a-zA-Z_][a-zA-Z_]*

NUM:
	| [0-9]+
```
