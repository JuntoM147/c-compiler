#include "lexer/lexer.hpp"

#include <iostream>

int main() {
    std::string input;

    std::getline(std::cin, input);

    Lexer lex(input);

    lex.print();

    return 0;
}

