#ifndef PARSER_H
#define PARSER_H
#include <vector>
#include <iostream>
#include <functional>
#include "http.h"
#include "toks.h"
#include "value.hpp"

namespace Asago {
    class Parser {
    public:
        void obtain_buffer(char * buffer, size_t sz);

        Value* load_from_file(std::string path);
        Value* load_from_url(std::string path);

        Value* read();
        void release_value(Value * val);
        Parser(std::vector<Token> toks);
        Parser() = default;

    private:
        std::vector<Token> toks;
        bool check_next(TOK_TYPE type, TOK_DET_TYPE det_type = TOK_DET_TYPE::UNDEF, std::string strval = "");
        bool check_last(TOK_TYPE type, TOK_DET_TYPE det_type = TOK_DET_TYPE::UNDEF, std::string strval = "");
        bool check_tok(const Token & tok, TOK_TYPE type, TOK_DET_TYPE det_type = TOK_DET_TYPE::UNDEF, std::string strval = "");
        Token next();

        Value * to_value(const Token & value);
        int load_number_int(std::string num);
        double load_number_dobule(std::string num);
        size_t tok_ptr = 0;
    };
}

#endif