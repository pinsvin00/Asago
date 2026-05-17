#ifndef TOK_DEF_H
#define TOK_DEF_H
#include <cstring>

namespace Asago {
    enum ASAGO_TOK_TYPE {
        ASAGO_VALUE,
        ASAGO_MODIFIER
    };

    enum ASAGO_TOK_DET_TYPE {
        ASAGO_UNDEF = -1,
        ASAGO_STRING,
        ASAGO_INT,
        ASAGO_INT_EXPONENT,
        ASAGO_FLOAT,
        ASAGO_FLOAT_EXPONENT,
        ASAGO_NIL,
        ASAGO_BOOL,
        ASAGO_COMMA,
        ASAGO_BRACKET_OPEN,
        ASAGO_BRACKET_CLOSE,
        ASAGO_ARR_OPEN,
        ASAGO_ARR_CLOSE,
        ASAGO_COLON,
    };

    struct Token {
        const char* tok_val;
        ASAGO_TOK_TYPE tok_type;
        ASAGO_TOK_DET_TYPE tok_det_type;
    };
}


#endif