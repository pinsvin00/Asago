#include "parser.h"

Value* Parser::read()
{
    Token initializer = next();
    if( !check_tok(initializer, TOK_TYPE::MODIFIER, TOK_DET_TYPE::BRACKET_OPEN) && 
        !check_tok(initializer, TOK_TYPE::MODIFIER, TOK_DET_TYPE::ARR_OPEN))
    {
        std::cout << "Expected { or [" << std::endl;
        exit(0);
    }

    Value * obj = new Value();
    if(initializer.tok_det_type == TOK_DET_TYPE::BRACKET_OPEN)
    {
        obj->mapped_values = new std::map<std::string, Value*>();
        obj->value_type = ValueType::OBJ;
    }
    else
    {
        obj->array_values = new std::vector<Value*>();
        obj->value_type = ValueType::ARRAY;
    }


    while(initializer.tok_det_type == TOK_DET_TYPE::BRACKET_OPEN)
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
            
            std::string key(identifier.tok_val);
            auto & deref = *obj->mapped_values;
            deref[key] = val(value);
        }
        else if(
            check_tok(value, TOK_TYPE::MODIFIER, TOK_DET_TYPE::BRACKET_OPEN) ||
            check_tok(value, TOK_TYPE::MODIFIER, TOK_DET_TYPE::ARR_OPEN) 
        )
        {
            tok_ptr--;
            Value * objo = read();
            std::string key(identifier.tok_val);
            auto & deref = *obj->mapped_values;
            deref[key] = objo;
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
    while (initializer.tok_det_type == ARR_OPEN)
    {
        Token value = next();
        if(check_tok(value, TOK_TYPE::VALUE))
        {
            auto & deref = *obj->array_values;
            deref.push_back(val(value));
        }
        else if(
            check_tok(value, TOK_TYPE::MODIFIER, TOK_DET_TYPE::BRACKET_OPEN) ||
            check_tok(value, TOK_TYPE::MODIFIER, TOK_DET_TYPE::ARR_OPEN) 
        )
        {
            tok_ptr--;
            Value * objo = read();
            auto & deref = *obj->array_values;
            deref.push_back(objo);
        }

        Token mod = next();
        if(check_tok(mod, MODIFIER, ARR_CLOSE))
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