/* Declaring two counters one for number
of lines other for number of characters */
%{
#include <vector>
#include <cstdlib>
#include <iostream>
#include <cstring> /* Added for strcmp, strlen, strdup */
#include <tok_def.h>
#define YY_NO_UNISTD_H
#ifdef _MSC_VER
#include <io.h>
#ifndef isatty
#define isatty _isatty
#endif
#endif

using namespace Asago;
std::vector<Token> ___toks;

char* substr(const char *src, int m, int n)
{
    int len = n - m;
    char *dest = (char*)malloc(sizeof(char) * (len + 1));
    for (int i = m; i < n && (*(src + i) != '\0'); i++)
    {
        *dest = *(src + i);
        dest++;
    }
    *dest = '\0';
    return dest - len;
}

Asago::ASAGO_TOK_DET_TYPE get_modifier_det_type(const char * val)
{
    if(strcmp(val, "{") == 0) return Asago::ASAGO_TOK_DET_TYPE::ASAGO_BRACKET_OPEN;
    if(strcmp(val, "}") == 0) return Asago::ASAGO_TOK_DET_TYPE::ASAGO_BRACKET_CLOSE;
    if(strcmp(val, ",") == 0) return Asago::ASAGO_TOK_DET_TYPE::ASAGO_COMMA;
    if(strcmp(val, ":") == 0) return Asago::ASAGO_TOK_DET_TYPE::ASAGO_COLON;
    if(strcmp(val, "[") == 0) return Asago::ASAGO_TOK_DET_TYPE::ASAGO_ARR_OPEN;
    if(strcmp(val, "]") == 0) return Asago::ASAGO_TOK_DET_TYPE::ASAGO_ARR_CLOSE;
    return Asago::ASAGO_TOK_DET_TYPE::ASAGO_UNDEF;
}
%}

/* Definitions Section */
INT     [0-9]+
FRAC    \.[0-9]+
EXP     [eE][+-]?[0-9]+
STRING  \"([^\\\"]|\\.)*\"

%%

[ \t\n\r]+    { /* Skip whitespace entirely so it doesn't interfere */ }

[+-]?[0-9]+(\.[0-9]+)?([eE][+-]?[0-9]+)? {
                            /* Raw regex inline to avoid macro evaluation bugs */
                            Token tok;
                            tok.tok_val = strdup(yytext);
                            
                            // Safer C-style check for float determination
                            if (strchr(yytext, '.') || strchr(yytext, 'e') || strchr(yytext, 'E')) {
                                tok.tok_det_type = Asago::ASAGO_TOK_DET_TYPE::ASAGO_FLOAT;
                            } else {
                                tok.tok_det_type = Asago::ASAGO_TOK_DET_TYPE::ASAGO_INT;
                            }
                            
                            tok.tok_type = Asago::ASAGO_TOK_TYPE::ASAGO_VALUE;
                            ___toks.push_back(tok);
                        }

{STRING} {
    Token tok;
    size_t strl = strlen(yytext);
    tok.tok_val = substr(yytext, 1, strl-1);
    tok.tok_det_type = Asago::ASAGO_TOK_DET_TYPE::ASAGO_STRING;
    tok.tok_type = Asago::ASAGO_TOK_TYPE::ASAGO_VALUE;
    ___toks.push_back(tok);
}

"null" {
    Token tok;
    tok.tok_val = "null";
    tok.tok_det_type = Asago::ASAGO_TOK_DET_TYPE::ASAGO_NIL;
    tok.tok_type = Asago::ASAGO_TOK_TYPE::ASAGO_VALUE;
    ___toks.push_back(tok);
}

"true"|"false" {
    Token tok;
    tok.tok_val = strdup(yytext);
    tok.tok_det_type = Asago::ASAGO_TOK_DET_TYPE::ASAGO_BOOL;
    tok.tok_type = Asago::ASAGO_TOK_TYPE::ASAGO_VALUE;
    ___toks.push_back(tok);
}

"{"|"}"|":"|","|"["|"]" {
    Token tok;
    tok.tok_val = strdup(yytext);
    tok.tok_type = Asago::ASAGO_TOK_TYPE::ASAGO_MODIFIER;
    tok.tok_det_type = get_modifier_det_type(tok.tok_val);
    ___toks.push_back(tok); 
}

. { /* Catch-all for any single unhandled characters (like stray + or - signs) */ 
    Token tok;
    tok.tok_val = strdup(yytext);
    tok.tok_type = Asago::ASAGO_TOK_TYPE::ASAGO_MODIFIER;
    tok.tok_det_type = Asago::ASAGO_TOK_DET_TYPE::ASAGO_UNDEF;
    ___toks.push_back(tok);
}

%%
 
int yywrap(){ return 1; }