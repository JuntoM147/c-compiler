#include <iostream>
#include "lexer/lexer.hpp"

int main() {
    std::string input;

    std::getline(std::cin, input);

    Lexer lex(input);

    lex.print();

    return 0;
}

