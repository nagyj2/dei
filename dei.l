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

/* COMMANDS - Top level instructions */
/* CALC -> math */
/* DEFINE -> <sym> : math */
/* DELETE ->  */
/* ANALYZE -> ? */

%%

  /* multichar matches - perform first to prevent 2 identical, sequential tokens */
"||"      { return UNION; }
"&&"      { return INTER; }
"//"      { return DIV; }
".."      { return RANGE; }

  /* single char matches */
"+"       |
"-"       |
"*"       |
"%"       |
"^"       |
"&"       |
"|"       |
","       |
"("       |
")"       |
"["       |
"]"       |
"{"       |
"}"       |
"d"       |
":"       { return yytext[0]; }

  /* special value matches */
    /* comparison operators */
">"       { yylval.fn = 1; return CMP; }
"<"       { yylval.fn = 2; return CMP; }
"!="      { yylval.fn = 3; return CMP; }
"=="      { yylval.fn = 4; return CMP; }
">="      { yylval.fn = 5; return CMP; }
"<="      { yylval.fn = 6; return CMP; }

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

    /* quantifiers */
"once"    { yylval.d = 1; return QUANT; }
"twice"   { yylval.d = 2; return QUANT; }
"thrice"  { yylval.d = 3; return QUANT; }
"always"  { yylval.d = 50; return QUANT; } /* TODO: use always as default, dependant on functype in dei.y */

"times"   { return XQUANT; }
"time"    { return XQUANT; }

"quit"    |
"q"       { return EXIT;}

  /* symbol names */
[a-zA-Z_][a-zA-Z_]*  { yylval.s = lookup(yytext); return IDENT; }

  /* number */
[0-9]+/"d"  { yylval.d = atoi(yytext); return DNUM; } /* detect die quantifiers different */
[0-9]+      { yylval.d = atoi(yytext); return NUM; }

  /* go into andf out of states */
"\\"                    { BEGIN(COMMENT); }
<COMMENT>"\\"           { BEGIN(INITIAL); }
<COMMENT>([^\\]|\n)+|.  { /* consume */ }

[ \t]     { /* ignore whitespace */ }
"\n"      { return EOL; }

.         { yyerror("unknown token: %c\n", *yytext); }

%%

  /* all     { yylval.fn = S_all; return SELECT; } */
