@page Grammar

```
math:
	|  math '+' math
	|  math '-' math
	|  math '*' math
	|  math '//' mat
	|  math '%' math
	|  math '^' math
	|  math '==' mat
	|  math '!=' mat
	|  math '>' math
	|  math '<' math
	|  math '>=' mat
	|  math '<=' mat
	|  '-' math
	|  '(' math ')'
	|  NUM
	|  IDENT
	|  set

set:  	
	|  set '&' set
	|  set '|' set
	|  set '&&' set
	|  set '||' set
	|  '[' set ']'
	|  func

func: 	
	|  die
	|  '{' list '}'
	|  func F_ADD a_args
	|  func F_SUB s_args
	|  func F_MOD m_args

a_args:
	|  squant ssel fquant
	|  psel fquant

s_args:
	|  squant ssel
	|  psel

m_args:
	|  squant ssel fquant
	|  squant ssel 'if' COND fquant
	|  psel fquant
	|  psel 'if' COND fquant

ssel:		
	|  SSELECT
	|  NUM

psel:
	|  PSELECT
	|  NUM 's'

squant:
	|  NUM
	|  SQUANT
	|  ''

fquant:
	|  NUM 'times'
	|  FQUANT
	|  ''

die: 		
	|  DNUM 'd' NUM
	|  DNUM 'd' '{' nnum RANGE nnum '}'
	|  'd' NUM
	|  'd' '{' nnum RANGE nnum '}'
	|  DNUM 'd' '{' list '}'
	|  'd' '{' list '}'

list: 	
	|  nnum
	|  nnum ',' list

nnum:		
	|  NUM
	|  '-' NUM

stmt:		
	|  stmt math EOL
	|  stmt IDENT ':' math EOL
	|  stmt error EOL
	|  stmt EOL
	|  '@' stmt math EOL
	|  '@' stmt IDENT ':' math EOL
	|  '@' stmt error EOL
	|  '@' stmt EOL
	|  ''

F_ADD:		
	|  'append'

F_SUB:		
	|  'drop'
	|  'count'
	|  'choose'

F_MOD:		
	|  'reroll'

COND:			
	|  'lower'
	|  'higher'

SSELECT:
	|  'highest'
	|  'lowest'
	|  'random'

PSELECT:
	|  'unique'

FQUANT:		
	|  'once'
	|  'twice'
	|  'thrice'

SQUANT:		
	|  'one'
	|  'two'
	|  'three'
	|  'four'
	|  'five'
```
