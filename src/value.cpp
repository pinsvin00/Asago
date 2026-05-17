#include "value.hpp"
using namespace Asago;

Value::~Value()
{
    if (dispose == true)
    {
        delete result;
        delete array_values;
        delete mapped_values;
    }
};
Value::Value(bool tf)
{
    this->value_type = ValueType::BOOL;
    this->result = new char[1];
    this->result_sz = 1;
    this->result[0] = (int)tf;
}
Value::Value(std::string value)
{
    this->value_type = ValueType::STRING;
    this->result = new char[value.size() + 1];
    this->result_sz = value.size() + 1; // include "\0"
    memcpy(this->result, value.c_str(), value.size() + 1);
}
Value::Value(double value)
{
    this->value_type = ValueType::DOUBLE;
    this->result = new char[sizeof(value)];
    this->result_sz = sizeof(value);
    memcpy(this->result, &value, sizeof(value));
}
Value::Value(int32_t value)
{
    this->value_type = ValueType::NUMBER;
    this->result = new char[sizeof(value)];
    this->result_sz = sizeof(value);
    memcpy(this->result, &value, sizeof(value));
}
Value::Value(char *arr, size_t sz, ValueType val_type, bool copy)
{
    this->value_type = val_type;
    this->result_sz = sz;
    if (copy)
    {
        this->result = new char[sz];
        memcpy(arr, this->result, sz);
    }
    else
    {
        this->result = arr;
    }
    this->result = arr;
}
Value::Value(Value *val, bool copy)
{
    this->value_type = val->value_type;
    this->result_sz = val->result_sz;
    if (val->value_type == ValueType::ARRAY)
    {
        if (copy)
        {
            this->array_values = new std::vector<Value *>();
            auto &values = *val->array_values;
            for (size_t i = 0; i < values.size(); i++)
            {
                this->array_values->push_back(new Value(values[i], copy));
            }
        }
        else
        {
            this->array_values = val->array_values;
        }
    }
    else if (val->value_type == ValueType::OBJ)
    {
        if (copy)
        {
            // implement this!
            this->mapped_values = val->mapped_values;
        }
        else
        {
            this->mapped_values = val->mapped_values;
        }
    }
    else
    {
        if (copy)
        {
            this->result = new char[result_sz];
            memcpy(this->result, val->result, result_sz);
        }
        else
        {
            this->result = val->result;
        }
        this->array_values = val->array_values;
    }
}

std::string Value::asString() const
{
    if (value_type == ValueType::STRING && result != nullptr)
    {
        return std::string(result);
    }
    if (value_type == ValueType::NUMBER && result != nullptr)
    {
        int32_t v = 0;
        memcpy(&v, result, sizeof(v));
        return std::to_string(v);
    }
    if (value_type == ValueType::DOUBLE && result != nullptr)
    {
        double d = 0.0;
        memcpy(&d, result, sizeof(d));
        return std::to_string(d);
    }
    if (value_type == ValueType::BOOL && result != nullptr)
    {
        return (result[0] != 0) ? "true" : "false";
    }
    return std::string();
}

int32_t Value::asInt() const
{
    if (value_type == ValueType::NUMBER && result != nullptr)
    {
        int32_t v = 0;
        memcpy(&v, result, sizeof(v));
        return v;
    }
    if (value_type == ValueType::DOUBLE && result != nullptr)
    {
        double d = 0.0;
        memcpy(&d, result, sizeof(d));
        return static_cast<int32_t>(d);
    }
    if (value_type == ValueType::STRING && result != nullptr)
    {
        try {
            return std::stoi(std::string(result));
        } catch(...) {
            return 0;
        }
    }
    if (value_type == ValueType::BOOL && result != nullptr)
    {
        return result[0] != 0 ? 1 : 0;
    }
    return 0;
}

double Value::asDouble() const
{
    if (value_type == ValueType::DOUBLE && result != nullptr)
    {
        double d = 0.0;
        memcpy(&d, result, sizeof(d));
        return d;
    }
    if (value_type == ValueType::NUMBER && result != nullptr)
    {
        int32_t v = 0;
        memcpy(&v, result, sizeof(v));
        return static_cast<double>(v);
    }
    if (value_type == ValueType::STRING && result != nullptr)
    {
        try {
            return std::stod(std::string(result));
        } catch(...) {
            return 0.0;
        }
    }
    if (value_type == ValueType::BOOL && result != nullptr)
    {
        return result[0] != 0 ? 1.0 : 0.0;
    }
    return 0.0;
}

bool Value::asBool() const
{
    if (value_type == ValueType::BOOL && result != nullptr)
    {
        return result[0] != 0;
    }
    if (value_type == ValueType::NUMBER && result != nullptr)
    {
        int32_t v = 0;
        memcpy(&v, result, sizeof(v));
        return v != 0;
    }
    if (value_type == ValueType::DOUBLE && result != nullptr)
    {
        double d = 0.0;
        memcpy(&d, result, sizeof(d));
        return d != 0.0;
    }
    if (value_type == ValueType::STRING && result != nullptr)
    {
        std::string s(result);
        return s == "true" || s == "1";
    }
    return false;
}

Value* Value::get_arr(int idx)
{
    if (array_values == nullptr) return nullptr;
    if (idx < 0) return nullptr;
    auto &arr = *array_values;
    if (static_cast<size_t>(idx) >= arr.size()) return nullptr;
    return arr[idx];
}

Value* Value::get_obj(const std::string &key)
{
    if (mapped_values == nullptr) return nullptr;
    auto it = mapped_values->find(key);
    if (it == mapped_values->end()) return nullptr;
    return it->second;
}