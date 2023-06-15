#include "parser.h"

Value* Parser::read()
{
    Token initializer = next();
    if(!check_tok(initializer, TOK_TYPE::MODIFIER, TOK_DET_TYPE::BRACKET_OPEN))
    {
        std::cout << "Expected {" << std::endl;
        exit(0);
    }

    Value * obj = new Value();
    obj->mapped_values = new std::map<std::string, Value*>();
    obj->value_type = ValueType::OBJ;

    while(true)
    {
        Token identifier = next();
        if(!check_tok(identifier, TOK_TYPE::VALUE, TOK_DET_TYPE::STRING))
        {
            std::cout << "Expected string value" << std::endl;
            exit(0);
        }

        Token colon = next();
        if(!check_tok(colon, TOK_TYPE::MODIFIER, TOK_DET_TYPE::COLON))
        {
            std::cout << "Expected \":\" token" << std::endl;
        }

        Token value = next();
        if(check_tok(value, TOK_TYPE::VALUE))
        {
            Value * val = new Value(value.tok_val);
            std::string key(identifier.tok_val);
            auto & deref = *obj->mapped_values;
            deref[key] = val;
        }
        else if(check_tok(value, TOK_TYPE::MODIFIER, TOK_DET_TYPE::BRACKET_OPEN))
        {
            Value * obj = read();
            std::string key(identifier.tok_val);
            auto & deref = *obj->mapped_values;
            deref[key] = obj;
        }

        Token mod = next();
        if(check_tok(mod, MODIFIER, BRACKET_CLOSE))
        {
            break;
        }
        else if(check_tok(mod, MODIFIER, COMMA))
        {
            continue;
        }
        else {
            std::cout  << "Expected , or }" << std::endl;
            exit(0);
        }

    }
    return obj;
}



bool Parser::check_tok(const Token & tok, TOK_TYPE type, TOK_DET_TYPE det_type, std::string strval)
{
    bool res = true;

    if(det_type != UNDEF && det_type != tok.tok_det_type)
    {
        res = false;
    }
    if(type != tok.tok_type)
    {
        res = false;
    }
    if(!strval.empty() && strval != tok.tok_val)
    {
        res = false;
    }
    return res;
}