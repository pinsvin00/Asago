cmake_minimum_required(VERSION 3.10)

# set the project name
project(Asago)
set(CMAKE_CXX_STANDARD 17)

# add_custom_command(
#     OUTPUT lex.yy.c
#     PRE_BUILD
#     COMMAND flex lexer.lex 
#     WORKING_DIRECTORY ${CMAKE_CURRENT_SOURCE_DIR}
#     DEPENDS tok_def.h
# )

set_source_files_properties(lex.yy.c PROPERTIES LANGUAGE CXX )
add_library(
    Asago 
    lex.yy.c
    parser.h
    parser.cpp
    toks.cpp
    toks.h
    http.h
    http.cpp
    util.cpp
    util.h
    tok_def.h
    value.hpp
    value.cpp
    ValuePrinter.cpp
    ValuePrinter.h
)

target_link_libraries(Asago curl)