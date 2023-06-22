#ifndef TOKS_H
#define TOKS_H
#include <vector>
#include <string>
#include "tok_def.h"

using namespace Asago;

bool check_last_tok(TOK_TYPE type, TOK_DET_TYPE det_type = UNDEF, std::string strval = "");



extern int   yylex();
extern char* yytext;
extern int   yyleng;
extern FILE * yyin;
typedef struct yy_buffer_state * YY_BUFFER_STATE;
extern YY_BUFFER_STATE yy_scan_string ( const char *str );
extern YY_BUFFER_STATE yy_scan_buffer(char *, size_t);
extern void yy_delete_buffer(YY_BUFFER_STATE buffer);

extern std::vector<Token> ___toks;

#endif