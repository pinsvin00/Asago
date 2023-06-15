#ifndef TOKS_H
#define TOKS_H
#include <vector>
#include <string>
#include "tok_def.h"

bool check_last_tok(TOK_TYPE type, TOK_DET_TYPE det_type = UNDEF, std::string strval = "");



extern int   yylex();
extern char* yytext;
extern int   yyleng;
extern FILE * yyin;
extern std::vector<Token> __toks;

#endif