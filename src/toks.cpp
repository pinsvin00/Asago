#include "toks.h"
using namespace Asago;
std::vector<Token> toks;

bool check_last_tok(ASAGO_TOK_TYPE type, ASAGO_TOK_DET_TYPE det_type, std::string strval)
{
    Token last_token = toks[toks.size()-1];
    bool res = true;

    if(det_type != ASAGO_UNDEF && det_type != last_token.tok_type)
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
