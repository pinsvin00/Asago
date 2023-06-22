#include "parser.h"
using namespace Asago;
bool Parser::check_next(TOK_TYPE type, TOK_DET_TYPE det_type, std::string strval)
{
    if (tok_ptr + 1 < toks.size())
    {
        return check_tok(toks[tok_ptr + 1], type, det_type, strval);
    }
    return false;
}
bool Parser::check_last(TOK_TYPE type, TOK_DET_TYPE det_type, std::string strval)
{
    if (tok_ptr != 0)
    {
        return check_tok(toks[tok_ptr], type, det_type, strval);
    }
    return false;
}
double Parser::load_number_dobule(std::string num)
{
    return std::stod(num);
}
int Parser::load_number_int(std::string num)
{
    return (int)std::stod(num);
}
Token Parser::next()
{
    if (tok_ptr < toks.size())
    {
        Token &tok = toks[tok_ptr];
        tok_ptr++;
        return tok;
    }
    return Token();
};

void Parser::obtain_buffer(char *buffer, size_t sz)
{
    yy_scan_string(buffer);
    yylex();
    for (auto &element : ___toks)
    {
        toks.push_back(element);
    }
}

Value* Parser::load_from_file(std::string path)
{
    FILE *fp;
    const char * fname = path.c_str();
    fp = fopen(fname,"r");
    yyin = fp;
    yylex();
    Parser * parser = new Parser(___toks);
    Value * obj = parser->read();
    return obj;
}

Value *Parser::load_from_url(std::string path)
{
    REST rest;
    Parser parser;
    rest.url = path.c_str();
    rest.onFinish = [&rest, &parser](char* buf, size_t sz){
        parser.obtain_buffer(buf, sz);
    };
    rest.run();

    Value* obj = parser.read();

    return obj;
}

Value *Parser::read()
{
    Token initializer = next();
    if (!check_tok(initializer, TOK_TYPE::MODIFIER, TOK_DET_TYPE::BRACKET_OPEN) &&
        !check_tok(initializer, TOK_TYPE::MODIFIER, TOK_DET_TYPE::ARR_OPEN))
    {
        std::cout << "Expected { or [" << std::endl;
        exit(0);
    }

    Value *obj = new Value();
    if (initializer.tok_det_type == TOK_DET_TYPE::BRACKET_OPEN)
    {
        obj->mapped_values = new std::map<std::string, Value *>();
        obj->value_type = ValueType::OBJ;
    }
    else
    {
        obj->array_values = new std::vector<Value *>();
        obj->value_type = ValueType::ARRAY;
    }

    while (initializer.tok_det_type == TOK_DET_TYPE::BRACKET_OPEN)
    {
        Token identifier = next();

        if (check_tok(identifier, MODIFIER, BRACKET_CLOSE))
        {
            return obj;
        }

        if (!check_tok(identifier, TOK_TYPE::VALUE, TOK_DET_TYPE::STRING))
        {
            std::cout << "Expected string value" << std::endl;
            exit(0);
        }
        Token colon = next();
        if (!check_tok(colon, TOK_TYPE::MODIFIER, TOK_DET_TYPE::COLON))
        {
            std::cout << "Expected \":\" token" << std::endl;
        }

        Token value = next();
        if (check_tok(value, TOK_TYPE::VALUE))
        {

            std::string key(identifier.tok_val);
            auto &deref = *obj->mapped_values;
            deref[key] = to_value(value);
        }
        else if (
            check_tok(value, TOK_TYPE::MODIFIER, TOK_DET_TYPE::BRACKET_OPEN) ||
            check_tok(value, TOK_TYPE::MODIFIER, TOK_DET_TYPE::ARR_OPEN))
        {
            tok_ptr--;
            Value *objo = read();
            std::string key(identifier.tok_val);
            auto &deref = *obj->mapped_values;
            deref[key] = objo;
        }

        Token mod = next();
        if (check_tok(mod, MODIFIER, BRACKET_CLOSE))
        {
            break;
        }
        else if (check_tok(mod, MODIFIER, COMMA))
        {
            continue;
        }
        else
        {
            std::cout << "Expected , or }" << std::endl;
            exit(0);
        }
    }
    while (initializer.tok_det_type == ARR_OPEN)
    {
        Token value = next();

        if (check_tok(value, MODIFIER, ARR_CLOSE))
        {
            return obj;
        }

        if (check_tok(value, TOK_TYPE::VALUE))
        {
            auto &deref = *obj->array_values;
            deref.push_back(to_value(value));
        }
        else if (
            check_tok(value, TOK_TYPE::MODIFIER, TOK_DET_TYPE::BRACKET_OPEN) ||
            check_tok(value, TOK_TYPE::MODIFIER, TOK_DET_TYPE::ARR_OPEN))
        {
            tok_ptr--;
            Value *objo = read();
            auto &deref = *obj->array_values;
            deref.push_back(objo);
        }

        Token mod = next();
        if (check_tok(mod, MODIFIER, ARR_CLOSE))
        {
            break;
        }
        else if (check_tok(mod, MODIFIER, COMMA))
        {
            continue;
        }
        else
        {
            std::cout << "Expected , or }" << std::endl;
            exit(0);
        }
    }

    return obj;
}
void Parser::release_value(Value *val)
{
    delete val->result;

    if(val->array_values != nullptr)
    {
        for(auto & element : *val->array_values)
        {
            release_value(element);
        }
        delete val->array_values;
    }
    if(val->mapped_values != nullptr)
    {
        for(auto & element : *val->mapped_values)
        {
            release_value(element.second);
        }
        delete val->mapped_values;
    }


}

Value *Parser::to_value(const Token &value)
{
    Value *val = nullptr;
    if (value.tok_det_type == TOK_DET_TYPE::STRING)
    {
        val = new Value(std::string(value.tok_val));
    }
    else if (value.tok_det_type == TOK_DET_TYPE::FLOAT)
    {
        val = new Value(load_number_dobule(value.tok_val));
    }
    else if (value.tok_det_type == TOK_DET_TYPE::INT)
    {
        val = new Value(load_number_int(value.tok_val));
    }
    else if (value.tok_det_type == TOK_DET_TYPE::BOOL)
    {
        val = new Value(value.tok_val == "true" ? true : false);
    }
    else if (value.tok_det_type == TOK_DET_TYPE::NIL)
    {
        val = new Value();
    }
    if (val == nullptr)
    {
        std::cout << "Invalid value" << value.tok_val << std::endl;
        exit(0);
    }
    return val;
}

    Parser::Parser(std::vector<Token> toks) {
        this->toks = toks;
    };

bool Parser::check_tok(const Token &tok, TOK_TYPE type, TOK_DET_TYPE det_type, std::string strval)
{
    bool res = true;

    if (det_type != UNDEF && det_type != tok.tok_det_type)
    {
        res = false;
    }
    if (type != tok.tok_type)
    {
        res = false;
    }
    if (!strval.empty() && strval != tok.tok_val)
    {
        res = false;
    }
    return res;
}