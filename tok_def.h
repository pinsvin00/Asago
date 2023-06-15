#ifndef TOK_DEF_H
#define TOK_DEF_H
#include <cstring>

enum TOK_TYPE {
    VALUE,
    MODIFIER
};

enum TOK_DET_TYPE {
    UNDEF = -1,
    STRING,
    INT,
    FLOAT,
    NIL,
    BOOL,
    COMMA,
    BRACKET_OPEN,
    BRACKET_CLOSE,
    ARR_OPEN,
    ARR_CLOSE,
    COLON,
};

struct Token {
    const char* tok_val;
    TOK_TYPE tok_type;
    TOK_DET_TYPE tok_det_type;
};


// TOK_DET_TYPE get_det_type_modifier(const char* str_val) {

// }

#endif