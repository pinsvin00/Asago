#include "toks.h"
using namespace Asago;
std::vector<Token> toks;

bool check_last_tok(TOK_TYPE type, TOK_DET_TYPE det_type, std::string strval)
{
    Token last_token = toks[toks.size()-1];
    bool res = true;

    if(det_type != UNDEF && det_type != last_token.tok_det_type)
    {
        res = false;
    }
    if(type != last_token.tok_type)
    {
        res = false;
    }
    if(strval != last_token.tok_val)
    {
        res = false;
    }
    return res;
}