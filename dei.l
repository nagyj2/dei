%option noyywrap

/*
  Condition "INITIAL" is the normal state
  %s Inclusive Condition : Use unmarked rules and condition rules
  %x Exclusive Condition : While active, only use condition rules
*/
%x COMMENT

%{
#include "dei.tab.h"
#include "dei.h"

%}

%%
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
"+"         | /*{ return PLUS; }*/
"-"         | /*{ return MINUS; }*/
"*"         | /*{ return MULT; }*/
"%"         | /*{ return MOD; }*/
"^"         | /*{ return EXPN; }*/
"&"         | /*{ return ELEMINTER; }*/
"|"         | /*{ return ELEMUNION; }*/
"="         | /*{ return EQ; }*/
">"         | /*{ return GT; }*/
"<"         | /*{ return LT; }*/
","         | /*{ return COMMA; }*/
"("         | /*{ return LPAREN; }*/
")"         | /*{ return RPAREN; }*/
"["         | /*{ return LBRACE; }*/
"]"         { return yytext[0]; } /*{ return RBRACE; }*/
".."        { return RANGE; }
"!="        { return NE; }
">="        { return GE; }
"<="        { return LE; }
"//"        { return DIV; }
"&&"        { return SETINTER; }
"||"        { return SETUNION; }
"d"         { return DIEFACE; }
"\n"        { return EOL; }
[0-9]+      { yylval.d = atoi(yytext); return NUM; }
[ \t]       { /* ignore whitespace */ }

                      /* go into andf out of states */
"\\"                  { BEGIN(COMMENT); }
<COMMENT>"\\"         { BEGIN(INITIAL); }
<COMMENT>([^\\]|\n)+|. { /* consume */ }

.           { printf("unknown: %c\n", *yytext); }
%%
