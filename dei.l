%option noyywrap nodefault yylineno
%{
#include "dei.tab.h"
#include "symboltable.h"
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
    /* selectors -> singular */
"highest" { yylval.fn = S_high; return SELECT; }
"lowest"  { yylval.fn = S_low; return SELECT; }
"random"  { yylval.fn = S_rand; return SELECT; }

  /* selectors -> plural */
"unique"  { yylval.fn = S_unique; return SELECT; }
  /* Normal number selectors are plural */

    /* temporal quantifiers */
"once"    { yylval.d = 1; return TQUANT; }
"twice"   { yylval.d = 2; return TQUANT; }
"thrice"  { yylval.d = 3; return TQUANT; }
"always"  { yylval.d = 50; return TQUANT; } /* TODO: use always as default, dependant on functype in dei.y */

    /* arbitrary temporal quantifier */
"times"   { return XQUANT; }
"time"    { return XQUANT; }

    /* singular quantifiers */
"one"   { yylval.d =  1; return SQUANT; }
"two"   { yylval.d =  2; return SQUANT; }
"three" { yylval.d =  3; return SQUANT; }
"four"  { yylval.d =  4; return SQUANT; }
"five"  { yylval.d =  5; return SQUANT; }
"six"   { yylval.d =  6; return SQUANT; }
"seven" { yylval.d =  7; return SQUANT; }
"eight" { yylval.d =  8; return SQUANT; }
"nine"  { yylval.d =  9; return SQUANT; }
"ten"   { yylval.d = 10; return SQUANT; }
"all"   { yylval.d = -1; return SQUANT; }


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
