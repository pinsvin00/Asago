#include <vector>
#include "parser.h"
#include "values.h"

int main()
{

    FILE *fp;
    const char * fname = "test.json";
    fp = fopen(fname,"r");
    yyin = fp;
    yylex();

    Parser * parser = new Parser(__toks);
    Value * val = parser->read();


    return 0;
}