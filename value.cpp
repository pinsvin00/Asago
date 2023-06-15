#include <vector>
#include <map>
#include <string>
#include <stdlib.h>
#include <cstring>

class Fragment {
public:
    std::string debug_value;
};

enum class FragmentType {
    UNDEF,
    VALUE
};

enum class ValueType {
    NUMBER,
    STRING,
    BOOL,
    ARRAY,
    UNSPECIFIED,
    OBJ,
};

class Value : public Fragment {
    public:
        ~Value() {
            if(dispose == true)
            {
                delete result;
                delete array_values;
                delete mapped_values;
            }
        };
        Value(bool tf) 
        {
            this->value_type = ValueType::BOOL;
            this->result = new char[1];
            this->result_sz = 1;
            this->result[0] = (int)tf;
        }
        Value(std::string value) {
            this->value_type = ValueType::STRING;
            this->result = new char[value.size() + 1];
            this->result_sz = value.size() + 1; //include "\0"
            memcpy(this->result, value.c_str(), value.size() + 1);
        }
        Value(int32_t value){
            this->value_type = ValueType::NUMBER;
            this->result = new char[sizeof(value)];
            this->result_sz = sizeof(value);
            memcpy(this->result, &value, sizeof(value));

        }
        Value(char* arr, size_t sz, ValueType val_type, bool copy=true)
        {
            this->value_type = val_type;
            this->result_sz = sz;
            if(copy)
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
        Value(Value* val, bool copy=true)
        {
            this->value_type = val->value_type;
            this->result_sz = val->result_sz;
            if(val->value_type == ValueType::ARRAY)
            {
                if(copy)
                {
                    this->array_values = new std::vector<Value*>();
                    auto& values = *val->array_values;
                    for(size_t i = 0; i < values.size() ; i++)
                    {
                        this->array_values->push_back(new Value(values[i], copy));
                    }
                }
                else
                {
                    this->array_values = val->array_values;
                }
            }
            else if(val->value_type == ValueType::OBJ)
            {
                if(copy)
                {
                    //implement this!
                    this->mapped_values = val->mapped_values;
                }
                else
                {
                    this->mapped_values = val->mapped_values;
                }
            }
            else
            {
                if(copy)
                {
                    this->result = new char[result_sz];
                    memcpy(this->result, val->result , result_sz);
                }
                else
                {
                    this->result = val->result;
                }
                this->array_values = val->array_values;
            }

        }

        Value() = default;
        ValueType value_type = ValueType::UNSPECIFIED;

        //If value is stored in some variable or is in code, it should persist
        bool dispose = true; 
        //There are some variables, than can be pre-maturily deleted
        bool is_mature = true;


        size_t result_sz = 0;
        char* result = nullptr;

        //array value
        std::vector<Value*> * array_values = nullptr;
        //todo add better indexing
        std::map<std::string, Value*> * mapped_values = nullptr;

};