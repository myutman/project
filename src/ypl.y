%{
	#include "model.h"

	//extern std::ofstream out;
	extern int yylineno;
	extern int yylex();
	Scope mainloop;
	void yyerror(char *s) {
		std::cerr << s << ", line " << yylineno << std::endl;
		exit(1);
	}
	#define YYSTYPE YYSTYPE_t
%}

%token IF ELSE DEF PRINT READ
%token NUM ID
%token OR AND EQ LE GE NE

%type<str> ID NUM
%type<ids> IDS
%type<args> OPS ARGS
%type<oper> OP UNARY EXPR

%left OR AND
%left EQ LE GE NE '<' '>'
%left '+' '-'
%left '*' '/' '%'

%%
PROGRAM:	OPS												{ out << "#include \"stdio.h\"\nint main() {\n";
																for (auto v: *$1){
																	v->gen_code(1, 0);
																	out << ";\n";	
																}
																out << "\treturn 0;\n}\n";
																//for (auto v: *$1) v->evaluate(mainloop); }
															}
;

OPS:		/* empty */										{ $$ = new vector<Object*>(); }
|			OPS OP ';'										{ $$ = $1; $$->push_back($2); }		
;

OP:			EXPR											//default
|			DEF ID '(' IDS ')' '{' OPS '}'					{ $$ = new FunctionDefinition($2, new FunctionAST($4, $7)); }
|			IF '(' EXPR ')' '{' OPS '}' ELSE '{' OPS '}'	{ $$ = new Conditional($3, $6, $10); }
|			IF '(' EXPR ')' '{' OPS '}'						{ $$ = new Conditional($3, $6); }
|			PRINT EXPR										{ $$ = new Print($2); }
|			READ ID											{ $$ = new Read($2); }
;

EXPR:		UNARY											// default
|			EXPR OR UNARY									{ $$ = new BinaryOperation($1, "||", $3); }
|			EXPR AND UNARY									{ $$ = new BinaryOperation($1, "&&", $3); }
|			EXPR EQ UNARY									{ $$ = new BinaryOperation($1, "==", $3); }
|			EXPR LE UNARY									{ $$ = new BinaryOperation($1, "<=", $3); }
|			EXPR GE UNARY									{ $$ = new BinaryOperation($1, ">=", $3); }
|			EXPR NE UNARY									{ $$ = new BinaryOperation($1, "!=", $3); }
|			EXPR '<' UNARY									{ $$ = new BinaryOperation($1, "<", $3); }
|			EXPR '>' UNARY									{ $$ = new BinaryOperation($1, ">", $3); }
|			EXPR '+' UNARY									{ $$ = new BinaryOperation($1, "+", $3); }
|			EXPR '-' UNARY									{ $$ = new BinaryOperation($1, "-", $3); }
|			EXPR '*' UNARY									{ $$ = new BinaryOperation($1, "*", $3); }
|			EXPR '/' UNARY									{ $$ = new BinaryOperation($1, "/", $3); }
|			EXPR '%' UNARY									{ $$ = new BinaryOperation($1, "%", $3); }
;

UNARY:		NUM 											{ $$ = new Number(atoi($1.c_str())); }
|			ID												{ $$ = new Reference($1); }
|			ID '(' ARGS ')'									{ $$ = new FunctionCall(new Reference($1), $3); }
|			'(' EXPR ')'									{ $$ = $2; }
|			'-' UNARY										{ $$ = new UnaryOperation("-", $2); }
|			'!' UNARY										{ $$ = new UnaryOperation("!", $2); }
;

ARGS:		/* empty */										{ $$ = new vector<Object*>(); }
|			EXPR											{ $$ = new vector<Object*>({$1}); }
|			ARGS ',' EXPR									{ $$ = $1; $$->push_back($3); }
;

IDS:		/* empty */										{ $$ = new vector<string>(); }
|			ID												{ $$ = new vector<string>({$1}); }
|			IDS ',' ID										{ $$ = $1; $$->push_back($3); }
;

%% 

//int main() { return yyparse(); }
