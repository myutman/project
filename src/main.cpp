#include "model.h"
#define YYSTYPE YYSTYPE_t
#include "../ypl.tab.h"
#include <cassert>

int main(int argc, char* argv[]){
    freopen(argv[1], "rt", stdin);
    return yyparse();
}
