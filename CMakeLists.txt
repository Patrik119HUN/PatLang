cmake_minimum_required(VERSION 3.22)
project(PatLang)

set(CMAKE_CXX_STANDARD 23)

add_executable(PatLang main.cpp
        tokenizer.h
        tokenizer.h
        parser.cpp
        parser.h
        tokenizer.cpp
        token_printer.h)
