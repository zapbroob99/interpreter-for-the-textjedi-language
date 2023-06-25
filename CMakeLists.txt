cmake_minimum_required(VERSION 3.25)
project(textjedi_interpreter C)

set(CMAKE_C_STANDARD 11)

add_executable(textjedi_interpreter main.c lexer.c lexer.h)
