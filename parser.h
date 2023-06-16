#ifndef PARSER_H
#define PARSER_H
#include <vector>
#include <iostream>
#include "toks.h"
#include "value.cpp"

class Parser {
public:
    int32_t bytes_to_int(char* buffer)
    {
        int32_t some = 0;
        some |= (unsigned char)buffer[0];
        some |= (unsigned char)buffer[1] << 8;
        some |= (unsigned char)buffer[2] << 16;
        some |= (unsigned char)buffer[3] << 24;

        return some;
    }

    double bytes_to_double(char* buffer)
    {
        double some;
        memcpy(&some, buffer, sizeof(double));
        return some;
    }

    std::string str(Value * value)
    {
        if(value->value_type == ValueType::NUMBER)
        {
            int32_t b = bytes_to_int(value->result);
            return std::to_string(b);
        }
        else if(value->value_type == ValueType::STRING)
        {
            return "\"" + std::string(value->result) + "\"";
        }
        else if(value->value_type == ValueType::DOUBLE)
        {
            double b = bytes_to_double(value->result);
            return std::to_string(b);
        }
        //This is very dangerous line, will cause infinite recursion
        else if(value->value_type == ValueType::ARRAY)
        {
            std::string buffer = "[ ";
            auto deref = value->array_values;
            for (size_t i = 0; i < deref->size(); i++)
            {
                buffer += str(value->array_values->at(i));
                buffer += ", ";
            }
            buffer.pop_back();
            buffer.pop_back();
            buffer += " ]";

            return buffer;
        }
        else if(value->value_type == ValueType::OBJ)
        {
            std::string buffer = "{";
            auto & deref = *value->mapped_values;
            for (auto pair : deref)
            {
                buffer +=  "\"" + pair.first + "\"" + " : " + str(pair.second) + " , ";
            }
            buffer.pop_back();
            buffer.pop_back();
            buffer += "}";

            return buffer;
        }
        else if(value->value_type == ValueType::BOOL)
        {
            char st = value->result[0];
            std::string value = st == 1 ? "true" : "false";
            return value;
        }
        return "";
    }

    Parser(std::vector<Token> toks) {
        this->toks = toks;
    };

    std::vector<Token> toks;
    bool check_next(TOK_TYPE type, TOK_DET_TYPE det_type = TOK_DET_TYPE::UNDEF, std::string strval = "")
    {
        if(tok_ptr + 1 < toks.size())
        {
            return check_tok(toks[tok_ptr + 1], type, det_type, strval);
        }
        return false;
    }
    bool check_last(TOK_TYPE type, TOK_DET_TYPE det_type = TOK_DET_TYPE::UNDEF, std::string strval = "")
    {
        if(tok_ptr != 0)
        {
            return check_tok(toks[tok_ptr], type, det_type, strval);
        }
        return false;
    }
    bool check_tok(const Token & tok, TOK_TYPE type, TOK_DET_TYPE det_type = TOK_DET_TYPE::UNDEF, std::string strval = "");
    Token next() {
        if(tok_ptr < toks.size())
        {
            Token & tok = toks[tok_ptr];
            tok_ptr++;
            return tok;
        }
        return Token();
    };

    Value * val(const Token & value)
    {
        Value * val = nullptr;
        if(value.tok_det_type == TOK_DET_TYPE::STRING)
        {
            val = new Value(std::string(value.tok_val));
        }
        else if(value.tok_det_type == TOK_DET_TYPE::FLOAT)
        {
            val = new Value(load_number_dobule(value.tok_val));
        }
        else if(value.tok_det_type == TOK_DET_TYPE::INT)
        {
            val = new Value(load_number_int(value.tok_val));
        }
        if(val == nullptr)
        {
            std::cout << "Invalid value" << value.tok_val << std::endl;
            exit(0);
        }
        return val;
    }

    int load_number_int(std::string num)
    {
        return (int)std::stod(num);
    }

    double load_number_dobule(std::string num)
    {
        return std::stod(num);
    }

    size_t tok_ptr = 0;

    Value* read();
};
#endif