#include <string>
#include "value.hpp"
#include "util.h"
#include <iostream>
#ifndef PRNTR_H
#define PRNTR_H
class ValuePrinter {
public:
    bool prettyPrint = false;
    std::string stringifyValue(Value * val);
    void printValue(Value * val);
    ValuePrinter() = default;
};
#endif