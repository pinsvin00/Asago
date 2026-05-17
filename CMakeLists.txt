cmake_minimum_required(VERSION 3.10)

# set the project name
project(Asago)
set(CMAKE_CXX_STANDARD 17)

add_custom_command(
    OUTPUT ${CMAKE_CURRENT_SOURCE_DIR}/src/lex.yy.c
    COMMAND flex -osrc/lex.yy.c src/lexer.lex
    WORKING_DIRECTORY ${CMAKE_CURRENT_SOURCE_DIR}
    DEPENDS ${CMAKE_CURRENT_SOURCE_DIR}/lexer.lex ${CMAKE_CURRENT_SOURCE_DIR}/inc/tok_def.h
    COMMENT "Generating src/lex.yy.c from lexer.lex"
)

add_custom_target(GenerateLexer DEPENDS ${CMAKE_CURRENT_SOURCE_DIR}/src/lex.yy.c)

set_source_files_properties(src/lex.yy.c PROPERTIES LANGUAGE CXX )

include_directories(inc)

add_library(
    Asago STATIC
    src/lex.yy.c
    src/AsagoParser.cpp
    src/toks.cpp
    src/util.cpp
    src/value.cpp
    src/ValuePrinter.cpp
)

add_dependencies(Asago GenerateLexer)

add_executable(
    AsagoTest
    src/main.cpp
)

target_link_libraries(AsagoTest Asago)
