#include "ValuePrinter.h"
using namespace Asago;

std::string ValuePrinter::stringifyValue(SEQL::Value *value)
{
    if (value->value_type == SEQL::ValueType::NUMBER)
    {
        int32_t b = asago_bytes_to_int(value->result);
        return std::to_string(b);
    }
    else if (value->value_type == SEQL::ValueType::NIL)
    {
        return "null";
    }
    else if (value->value_type == SEQL::ValueType::STRING)
    {
        return "\"" + std::string(value->result) + "\"";
    }
    else if (value->value_type == SEQL::ValueType::DOUBLE)
    {
        double b = asago_bytes_to_double(value->result);
        return std::to_string(b);
    }
    // This is very dangerous line, will cause infinite recursion
    else if (value->value_type == SEQL::ValueType::ARRAY)
    {
        std::string buffer = "[";
        auto deref = value->array_values;
        for (size_t i = 0; i < deref->size(); i++)
        {
            buffer += stringifyValue(value->array_values->at(i));
            buffer += ", ";
        }
        if (buffer.size() >= 2)
        {
            buffer.pop_back();
            buffer.pop_back();
        }

        buffer += " ]";

        return buffer;
    }
    else if (value->value_type == SEQL::ValueType::OBJ)
    {
        std::string buffer = "{";
        auto &deref = *value->mapped_values;
        for (auto pair : deref)
        {
            buffer += "\"" + pair.first + "\"" + " : " + stringifyValue(pair.second) + " , ";
        }
        if (buffer.size() >= 2)
        {
            buffer.pop_back();
            buffer.pop_back();
        }
        buffer += "}";

        return buffer;
    }
    else if (value->value_type == SEQL::ValueType::BOOL)
    {
        char st = value->result[0];
        std::string value = st == 1 ? "true" : "false";
        return value;
    }
    return "";
}

void ValuePrinter::printValue(SEQL::Value *val)
{
    std::cout << this->stringifyValue(val) << std::endl;
}
