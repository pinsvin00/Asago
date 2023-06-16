#include <vector>
#include "parser.h"
#include "values.h"

int main()
{

    FILE *fp;
    const char * fname = "weather.json";
    fp = fopen(fname,"r");
    yyin = fp;
    yylex();
    Parser * parser = new Parser(__toks);
    Value * obj = parser->read();
    
    std::cout << parser->str(obj) << std::endl;

    return 0;
}