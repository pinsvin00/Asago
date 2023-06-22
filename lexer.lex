/* Declaring two counters one for number
of lines other for number of characters */
%{
#include <vector>
#include <cstdlib>
#include <iostream>
#include "tok_def.h"
using namespace Asago;
std::vector<Token> ___toks;

char* substr(const char *src, int m, int n)
{
    // get the length of the destination string
    int len = n - m;
 
    // allocate (len + 1) chars for destination (+1 for extra null character)
    char *dest = (char*)malloc(sizeof(char) * (len + 1));
 
    // extracts characters between m'th and n'th index from source string
    // and copy them into the destination string
    for (int i = m; i < n && (*(src + i) != '\0'); i++)
    {
        *dest = *(src + i);
        dest++;
    }
 
    // null-terminate the destination string
    *dest = '\0';
 
    // return the destination string
    return dest - len;
}

TOK_DET_TYPE get_modifier_det_type(const char * val)
{
    if(strcmp(val, "{") == 0)
    {
        return TOK_DET_TYPE::BRACKET_OPEN;
    }
    if(strcmp(val, "}") == 0)
    {
        return TOK_DET_TYPE::BRACKET_CLOSE;
    }
    if(strcmp(val, ",") == 0)
    {
        return TOK_DET_TYPE::COMMA;
    }
    if(strcmp(val, ":") == 0)
    {
        return TOK_DET_TYPE::COLON;
    }
    if(strcmp(val, "[") == 0)
    {
        return TOK_DET_TYPE::ARR_OPEN;
    }
    if(strcmp(val, "]") == 0)
    {
        return TOK_DET_TYPE::ARR_CLOSE;
    }
    return TOK_DET_TYPE::UNDEF;
} 

%}

NUMBER   [0-9][0-9]*
EXPONENT [eE](\+\-)?[1-9]\d*
STRING   \"([^\\\"]|\\.)*\"

%%
{NUMBER}+{EXPONENT} {
    Token tok;
    tok.tok_val = strdup(yytext);
    tok.tok_det_type = TOK_DET_TYPE::INT;
    tok.tok_type = TOK_TYPE::VALUE;
    ___toks.push_back(tok);
}

{NUMBER} {
    Token tok;
    tok.tok_val = strdup(yytext);
    tok.tok_det_type = TOK_DET_TYPE::INT;
    tok.tok_type = TOK_TYPE::VALUE;
    ___toks.push_back(tok);
}



{NUMBER}+"."[0-9]* {
    Token tok;
    tok.tok_val = strdup(yytext);
    tok.tok_det_type = TOK_DET_TYPE::FLOAT;
    tok.tok_type = TOK_TYPE::VALUE;
    ___toks.push_back(tok);
}

{NUMBER}+"."[0-9]*+{EXPONENT} {
    Token tok;
    tok.tok_val = strdup(yytext);
    tok.tok_det_type = TOK_DET_TYPE::FLOAT;
    tok.tok_type = TOK_TYPE::VALUE;
    ___toks.push_back(tok);
}


{STRING} {
    Token tok;
    size_t strl = strlen(yytext);
    tok.tok_val = substr(yytext, 1, strl-1);
    tok.tok_det_type = TOK_DET_TYPE::STRING;
    tok.tok_type = TOK_TYPE::VALUE;

    ___toks.push_back(tok);
}

"null" {
    Token tok;
    tok.tok_val = "null";
    tok.tok_det_type = TOK_DET_TYPE::NIL;
    tok.tok_type = TOK_TYPE::VALUE;

    ___toks.push_back(tok);
}

"true"|"false" {
    Token tok;
    tok.tok_val = strdup(yytext);
    tok.tok_det_type = TOK_DET_TYPE::BOOL;
    tok.tok_type = TOK_TYPE::VALUE;

    ___toks.push_back(tok);
}

"{"|"}"|":"|","|"["|"]" {
    Token tok;
    tok.tok_val = strdup(yytext);
    tok.tok_type = TOK_TYPE::MODIFIER;
    tok.tok_det_type = get_modifier_det_type(tok.tok_val);
    ___toks.push_back(tok); 
}

%%
 
/*** User code section***/
int yywrap(){}