#include "AsagoParser.h"
using namespace Asago;
bool Parser::check_next(ASAGO_TOK_TYPE type, ASAGO_TOK_DET_TYPE det_type, std::string strval)
{
    if (tok_ptr + 1 < toks.size())
    {
        return check_tok(toks[tok_ptr + 1], type, det_type, strval);
    }
    return false;
}
bool Parser::check_last(ASAGO_TOK_TYPE type, ASAGO_TOK_DET_TYPE det_type, std::string strval)
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
    return std::stoi(num);
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

Value *Parser::read()
{
    Token initializer = next();
    if (!check_tok(initializer, ASAGO_TOK_TYPE::ASAGO_MODIFIER, ASAGO_TOK_DET_TYPE::ASAGO_BRACKET_OPEN) &&
        !check_tok(initializer, ASAGO_TOK_TYPE::ASAGO_MODIFIER, ASAGO_TOK_DET_TYPE::ASAGO_ARR_OPEN))
    {
        std::cout << "Expected { or [" << std::endl;
        exit(0);
    }

    Value *obj = new Value();
    if (initializer.tok_det_type == ASAGO_TOK_DET_TYPE::ASAGO_BRACKET_OPEN)
    {
        obj->mapped_values = new std::map<std::string, Value *>();
        obj->value_type = ValueType::OBJ;
    }
    else
    {
        obj->array_values = new std::vector<Value*>();
        obj->value_type = ValueType::ARRAY;
    }

    while (initializer.tok_det_type == ASAGO_TOK_DET_TYPE::ASAGO_BRACKET_OPEN)
    {
        Token identifier = next();

        if (check_tok(identifier, ASAGO_MODIFIER, ASAGO_BRACKET_CLOSE))
        {
            return obj;
        }

        if (!check_tok(identifier, ASAGO_TOK_TYPE::ASAGO_VALUE, ASAGO_TOK_DET_TYPE::ASAGO_STRING))
        {
            std::cout << "Expected ASAGO_STRING ASAGO_VALUE" << std::endl;
            exit(0);
        }
        Token ASAGO_COLON = next();
        if (!check_tok(ASAGO_COLON, ASAGO_TOK_TYPE::ASAGO_MODIFIER, ASAGO_TOK_DET_TYPE::ASAGO_COLON))
        {
            std::cout << "Expected \":\" token" << std::endl;
        }

        Token ASAGO_VALUE = next();
        if (check_tok(ASAGO_VALUE, ASAGO_TOK_TYPE::ASAGO_VALUE))
        {

            std::string key(identifier.tok_val);
            auto &deref = *obj->mapped_values;
            deref[key] = to_value(ASAGO_VALUE);
        }
        else if (
            check_tok(ASAGO_VALUE, ASAGO_TOK_TYPE::ASAGO_MODIFIER, ASAGO_TOK_DET_TYPE::ASAGO_BRACKET_OPEN) ||
            check_tok(ASAGO_VALUE, ASAGO_TOK_TYPE::ASAGO_MODIFIER, ASAGO_TOK_DET_TYPE::ASAGO_ARR_OPEN))
        {
            tok_ptr--;
            Value *objo = read();
            std::string key(identifier.tok_val);
            auto &deref = *obj->mapped_values;
            deref[key] = objo;
        }

        Token mod = next();
        if (check_tok(mod, ASAGO_MODIFIER, ASAGO_BRACKET_CLOSE))
        {
            break;
        }
        else if (check_tok(mod, ASAGO_MODIFIER, ASAGO_COMMA))
        {
            continue;
        }
        else
        {
            std::cout << "Expected , or }" << std::endl;
            exit(0);
        }
    }
    while (initializer.tok_det_type == ASAGO_ARR_OPEN)
    {
        Token asago_value = next();

        if (check_tok(asago_value, ASAGO_MODIFIER, ASAGO_ARR_CLOSE))
        {
            return obj;
        }

        if (check_tok(asago_value, ASAGO_TOK_TYPE::ASAGO_VALUE))
        {
            auto &deref = *obj->array_values;
            deref.push_back(to_value(asago_value));
        }
        else if (
            check_tok(asago_value, ASAGO_TOK_TYPE::ASAGO_MODIFIER, ASAGO_TOK_DET_TYPE::ASAGO_BRACKET_OPEN) ||
            check_tok(asago_value, ASAGO_TOK_TYPE::ASAGO_MODIFIER, ASAGO_TOK_DET_TYPE::ASAGO_ARR_OPEN))
        {
            tok_ptr--;
            Value *objo = read();
            auto &deref = *obj->array_values;
            deref.push_back(objo);
        }

        Token mod = next();
        if (check_tok(mod, ASAGO_MODIFIER, ASAGO_ARR_CLOSE))
        {
            break;
        }
        else if (check_tok(mod, ASAGO_MODIFIER, ASAGO_COMMA))
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

Value *Parser::to_value(const Token &asago_value)
{
    Value *val = nullptr;
    if (asago_value.tok_det_type == ASAGO_TOK_DET_TYPE::ASAGO_STRING)
    {
        val = new Value(std::string(asago_value.tok_val));
    }
    else if (asago_value.tok_det_type == ASAGO_TOK_DET_TYPE::ASAGO_FLOAT)
    {
        val = new Value(load_number_dobule(asago_value.tok_val));
    }
    else if (asago_value.tok_det_type == ASAGO_TOK_DET_TYPE::ASAGO_INT)
    {
        val = new Value(load_number_int(asago_value.tok_val));
    }
    else if (asago_value.tok_det_type == ASAGO_TOK_DET_TYPE::ASAGO_BOOL)
    {
        val = new Value(asago_value.tok_val == "true" ? true : false);
    }
    else if (asago_value.tok_det_type == ASAGO_TOK_DET_TYPE::ASAGO_NIL)
    {
        val = new Value();
    }
    if (val == nullptr)
    {
        std::cout << "Invalid ASAGO_VALUE" << asago_value.tok_val << std::endl;
        exit(0);
    }
    return val;
}

    Parser::Parser(std::vector<Token> toks) {
        this->toks = toks;
    };

bool Parser::check_tok(const Token &tok, ASAGO_TOK_TYPE type, ASAGO_TOK_DET_TYPE det_type, std::string strval)
{
    bool res = true;

    if (det_type != ASAGO_UNDEF && det_type != tok.tok_det_type)
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
