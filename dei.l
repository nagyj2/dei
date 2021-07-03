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
  /* single char matches */
"+"         |
"-"         |
"*"         |
"%"         |
"^"         |
"&"         |
"|"         |
","         |
"("         |
")"         |
"["         |
"]"         |
"d"         |
":"         { return yytext[0]; }

  /* multichar matches */
"||"        { return UNION; }
"&&"        { return INTER; }
"//"        { return DIV; }
".."      { return RANGE; }

  /* special value matches */
    /* comparison operators */
">"         { yylval.fn = 1; return CMP; }
"<"         { yylval.fn = 2; return CMP; }
"!="        { yylval.fn = 3; return CMP; }
"=="        { yylval.fn = 4; return CMP; }
">="        { yylval.fn = 5; return CMP; }
"<="        { yylval.fn = 6; return CMP; }

  /* keywords */
    /* functions */
"drop"    { yylval.fn = B_drop; return FUNC; }
"append"  { yylval.fn = B_append; return FUNC; }
"choose"  { yylval.fn = B_choose; return FUNC; }
"reroll"  { yylval.fn = B_reroll; return FUNC; }
"count"   { yylval.fn = B_count; return FUNC; }
    /* selectors */
"highest" { yylval.fn = S_high; return SELECT; }
"lowest"  { yylval.fn = S_low; return SELECT; }
"random"  { yylval.fn = S_rand; return SELECT; }
"unique"  { yylval.fn = S_unique; return SELECT; }
"all"     { yylval.fn = S_all; return SELECT; }
    /* quantifiers */
"once"    { yylval.d = 1; return TIMES; }
"twice"   { yylval.d = 2; return TIMES; }
"thrice"  { yylval.d = 3; return TIMES; }

  /* names */
[a-zA-Z_][a-zA-Z_]*  { /*TODO ylval.s = lookup(yytext); */ return IDENT; }

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
