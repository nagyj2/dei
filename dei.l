%option noyywrap nodefault yylineno
%{
#include "dei.tab.h"
#include "dei.h"
%}

/*
  Condition "INITIAL" is the normal state
  %s Inclusive Condition : Use unmarked rules and condition rules
  %x Exclusive Condition : While active, only use condition rules
*/
%x COMMENT

%%
  /* single char ops */
"+"         | /*{ return PLUS; }*/
"-"         | /*{ return MINUS; }*/
"*"         | /*{ return MULT; }*/
"%"         | /*{ return MOD; }*/
"^"         | /*{ return EXPN; }*/
"&"         | /*{ return ELEMINTER; }*/
"|"         | /*{ return ELEMUNION; }*/
","         | /*{ return COMMA; }*/
"("         | /*{ return LPAREN; }*/
")"         | /*{ return RPAREN; }*/
"["         | /*{ return LBRACE; }*/
"="         |
";"         |
"]"         { return yytext[0]; } /*{ return RBRACE; }*/

  /* multichar ops */
"//"        { return DIV; }

  /* comparison operators */
">"         { yylval.fn = 1; return CMP; } /*{ return GT; }*/
"<"         { yylval.fn = 2; return CMP; } /*{ return LT; }*/
"!="        { yylval.fn = 3; return CMP; } /*{ return NE; }*/
"=="        { yylval.fn = 4; return CMP; } /*{ return EQ; }*/
">="        { yylval.fn = 5; return CMP; } /*{ return GE; }*/
"<="        { yylval.fn = 6; return CMP; } /*{ return LE; }*/

  /* keywords */
"if"        { return IF; }
"then"      { return THEN; }
"else"      { return ELSE; }
"while"     { return WHILE; }
"do"        { return DO; }
"let"       { return LET; }

  /* built-in functions */
"sqrt"      { yylval.fn = B_sqrt; return FUNC; }
"exp"       { yylval.fn = B_exp; return FUNC; }
"log"       { yylval.fn = B_log; return FUNC; }
"print"     { yylval.fn = B_print; return FUNC; }

  /* names */
[a-zA-Z][a-zA-Z0-9]*  { yylval.s = lookup(yytext); return IDENT; }

  /* number */
[0-9]+      { yylval.d = atoi(yytext); return NUM; }

  /* go into andf out of states */
"\\"                  { BEGIN(COMMENT); }
<COMMENT>"\\"         { BEGIN(INITIAL); }
<COMMENT>([^\\]|\n)+|. { /* consume */ }

[ \t]       { /* ignore whitespace */ }
"\n"        { return EOL; }

.           { yyerror("unknown token: %c\n", *yytext); }

%%
/*
"drop"      { return DROP; }
"append"    { return APPEND; }
"choose"    { return CHOOSE; }
"reroll"    { return REROLL; }
"count"     { return COUNT; }
"highest"   { return HIGH; }
"lowest"    { return LOW; }
"random"    { return RAND; }
"unique"    { return SET; }
"all"       { return ALL; }
"once"      { yylval.d = 1; return TIME; }
"twice"     { yylval.d = 2; return TIME; }
"thrice"    { yylval.d = 3; return TIME; }
".."        { return RANGE; }
"&&"        { return SETINTER; }
"||"        { return SETUNION; }
"d"         { return DIEFACE; }
*/
