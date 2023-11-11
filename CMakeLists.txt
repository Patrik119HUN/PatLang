cmake_minimum_required(VERSION 3.22)
project(PatLang)
set(CMAKE_CXX_STANDARD 17)
include_directories(PatLang ${PROJECT_SOURCE_DIR}/header)
add_executable(PatLang main.cpp
        tokenizer.h
        parser.h
        ASTNode.h
        symbol_table.h
        visitor_interface.h
        code_generator.h
        node_interface.h
)
