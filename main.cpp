#include <vector>
#include "parser.h"
#include "values.h"
#include "http.h"
#include "ValuePrinter.h"

int main()
{
    Parser p;
    ValuePrinter prntr;
    Value * v = p.load_from_url("https://jsonplaceholder.typicode.com/todos/20");
    prntr.printValue(v);

    p.release_value(v);

    return 0;
}