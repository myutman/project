#include "model.h"
#define YYSTYPE YYSTYPE_t
#include "../ypl.tab.h"
#include <cassert>
#include <cstring>

int main(int argc, char* argv[]){
	if (argc < 2){
		cerr << "invalid number of arguments";
		return -1;
	}
	freopen(argv[1], "rt", stdin);
	std::string s;
	if (argc == 2){
		s = "a.out";
	} else {
		s = argv[2];
	}
	out.open("tmp.c", std::ofstream::out);
	int exit_code = yyparse();
	out.close();
	string ar = "gcc tmp.c -o " + s;
	system(ar.c_str());
	system("rm tmp.c");
	return exit_code;
}
