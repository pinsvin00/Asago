#ifndef PARSER_H
#define PARSER_H
#include <vector>
#include <iostream>
#include "toks.h"
#include "value.cpp"

class Parser {
public:
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

    size_t tok_ptr = 0;

    Value* read();
};
#endif