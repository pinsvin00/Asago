#include <vector>
#include "AsagoParser.h"
#include "value.hpp"
#include "ValuePrinter.h"

int main()
{
    Asago::Parser p;
    ValuePrinter prntr;
    Value * v = p.load_from_file("test.json");
    prntr.printValue(v);

    p.release_value(v);

    return 0;
}