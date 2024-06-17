cmake_minimum_required(VERSION 3.28)
project(velkauloha)

set(CMAKE_CXX_STANDARD 20)

add_compile_options(-Wall -pedantic -g -Wextra -Wsuggest-attribute=const)

add_executable(velkauloha main.cpp
        CPos.cpp
        CPos.h
        StackExpressionvalue.h
        SinglePartOfExpression.cpp
        SinglePartOfExpression.h
        Sell.cpp
        Sell.h
        Parser.cpp
        Parser.h
        CSpreadsheet.cpp
        CSpreadsheet.h
)
target_link_libraries(velkauloha ${CMAKE_SOURCE_DIR}/x86_64-linux-gnu/libexpression_parser.a)
