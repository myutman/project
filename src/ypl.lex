%{
	#include <string>
	#include "model.h"
	#define YYSTYPE YYSTYPE_t
	#include "ypl.tab.h"
	void yyerror(char* s);
%}

%option yylineno
%option noyywrap

%%

[/][/].*\n      ; // comment
if				return IF;
else			return ELSE;
def				return DEF;
print			return PRINT;
read			return READ;
[|][|]			return OR;
&&				return AND;
==				return EQ;
[<]=			return LE;
>=				return GE;
!=				return NE;
[0-9]+			{ yylval.str = yytext; return NUM; }
[a-zA-Z_][a-zA-Z0-9_]* { yylval.str = yytext; return ID; }
[ \t\r\n]		; // whitespace
[-{};()=<>+*/!,] { return *yytext; }
.				yyerror("Invalid character");

%%
