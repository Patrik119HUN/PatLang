cmake_minimum_required(VERSION 3.22)
project(PatLang)
set(CMAKE_CXX_STANDARD 17)
include_directories(PatLang ${PROJECT_SOURCE_DIR}/header)
add_executable(PatLang main.cpp
        tokenizer.h
        tokenizer.cpp
        parser.h
        parser.cpp
        token_printer.h
        header/ASTNode.h
        symbol_table.h
        header/visitor_interface.h
        header/code_generator.h
)
