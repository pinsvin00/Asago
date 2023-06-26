// #include <vector>
// #include <map>
// #include <string>
// #include <stdlib.h>
// #include <cstring>
// #ifndef VALUE_ASAGO_H
// #define VALUE_ASAGO_H

// namespace Asago {
// class Fragment {
// public:
//     std::string debug_value;
// };

// enum class FragmentType {
//     UNDEF,
//     VALUE
// };

// enum class ValueType {
//     NUMBER,
//     STRING,
//     BOOL,
//     ARRAY,
//     UNSPECIFIED,
//     NIL,
//     DOUBLE,
//     OBJ,
// };

// class Value : public Fragment {
//     public:
//         ~Value();
//         Value(bool tf) ;
//         Value(std::string value);
//         Value(double value);
//         Value(int32_t value);
//         Value(char* arr, size_t sz, ValueType val_type, bool copy=true);
//         Value(Value* val, bool copy=true);

//         Value() = default;
//         ValueType value_type = ValueType::UNSPECIFIED;

//         //If value is stored in some variable or is in code, it should persist
//         bool dispose = true; 
//         //There are some variables, than can be pre-maturily deleted
//         bool is_mature = true;


//         size_t result_sz = 0;
//         char* result = nullptr;

//         //array value
//         std::vector<Value*> * array_values = nullptr;
//         //todo add better indexing
//         std::map<std::string, Value*> * mapped_values = nullptr;

// };
// }


// #endif