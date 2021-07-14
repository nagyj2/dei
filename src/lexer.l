%option noyywrap nodefault yylineno
%{

#include "parser.tab.h"

#include "deimain.h"

%}

	/* consume until an end is met */
%x COMMENT

%%

	/* multichar matches - perform first to prevent 2 identical, sequential tokens */
"||"      { return UNION; }
"&&"      { return INTER; }
"//"      { return DIV; }
".."      { return RANGE; }
"if"			{ return IF; }

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
"s"       |
";"				|
"@"				| /* mute */
":"       { return yytext[0]; }

	/* special value matches */
		/* comparison operators */
">"       { /*yylval.fn = 1;*/ return CMP; }
"<"       { /*yylval.fn = 2;*/ return CMP; }
"!="      { /*yylval.fn = 3;*/ return CMP; }
"=="      { /*yylval.fn = 4;*/ return CMP; }
">="      { /*yylval.fn = 5;*/ return CMP; }
"<="      { /*yylval.fn = 6;*/ return CMP; }

	/* keywords */
		/* additive */
"append"  { /*yylval.fn = B_append;*/ return F_ADD; }
		/* subtractive */
"drop"    { /*yylval.fn = B_drop;*/ return F_SUB; }
"choose"  { /*yylval.fn = B_choose;*/ return F_SUB; }
"count"   { /*yylval.fn = B_count;*/ return F_SUB; }
		/* moditive */
"reroll"  { /*yylval.fn = B_reroll;*/ return F_MOD; }

		/* selectors -> singular */
"highest" { /*yylval.fn = S_high;*/ return SSELECT; }
"lowest"  { /*yylval.fn = S_low;*/ return SSELECT; }
"random"  { /*yylval.fn = S_rand;*/ return SSELECT; }

		/* selectors -> plural */
"unique"  { /*yylval.fn = S_unique;*/ return PSELECT; }
	/* Normal number selectors are plural */

		/* temporal quantifiers */
"once"    { /*yylval.d = 1;*/ return FQUANT; }
"twice"   { /*yylval.d = 2;*/ return FQUANT; }
"thrice"  { /*yylval.d = 3;*/ return FQUANT; }

		/* arbitrary temporal quantifier */
"times"   { return XQUANT; }
"time"    { return XQUANT; }

		/* singular quantifiers */
"one"   { /*yylval.d =  1;*/ return SQUANT; }
"two"   { /*yylval.d =  2;*/ return SQUANT; }
"three" { /*yylval.d =  3;*/ return SQUANT; }
"four"  { /*yylval.d =  4;*/ return SQUANT; }
"five"  { /*yylval.d =  5;*/ return SQUANT; }

		/* conditionals */
"higher"	{ /*yylval.fn = C_high;*/ return COND; }
"lower"		{ /*yylval.fn = C_low;*/ return COND; }

"quit"    |
"q"       { return EXIT;}

	/* symbol names */
[a-zA-Z_][a-zA-Z_]*  { /*lookup(yytext);*/ return IDENT; }

	/* number */
[0-9]+/"d"  { /*yylval.d = atoi(yytext);*/ return DNUM; } /* detect die quantifiers different */
[0-9]+      { /*yylval.d = atoi(yytext);*/ return NUM; }

	/* go into andf out of states */
"\\"                    { BEGIN(COMMENT); }
<COMMENT>"\\"           { BEGIN(INITIAL); }
<COMMENT>([^\\]|\n)+|.  { /* consume */ }

[ \t]     { /* ignore whitespace */ }
"\n"      { return EOL; }
<<EOF>>		{ return 0; }

.         { yyerror("unknown token: %c\n", *yytext); }

%%
