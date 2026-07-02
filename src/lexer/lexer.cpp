#include <cctype>
#include <iostream>
#include <cassert>
#include "lexer.hpp"

bool is_whitespace(char curr) {
    return curr == ' ' || curr == '\t' || curr == '\n';
}


char Lexer::peek()
{
    assert(_index < _input.size());
    return _input.at(_index);
}


bool Lexer::is_at_end()
{
    return _index >= _input.size();
}

void Lexer::advance()
{
    _index++;
}


void Lexer::lex_identifier_or_keyword()
{
    size_t start = _index;

    while (!is_at_end() && (std::isalnum(peek()) || peek() == '_')) {
        advance();
    }
    
    std::string value = _input.substr(start, _index - start);

    if (value == "int") {
        _tokens.push_back(Token(
                    TokenType::keyword_int,
                    value
                    ));
    } else if (value == "void") {
        _tokens.push_back(Token(
                    TokenType::keyword_void,
                    value
                    ));
    } else if (value == "return") {
        _tokens.push_back(Token(
                    TokenType::keyword_return,
                    value
                    ));
    } else {
        _tokens.push_back(Token(
                    TokenType::identifier,
                    value
                    ));
    }
}


void Lexer::lex_constant()
{
    size_t start = _index;

    while (!is_at_end() && std::isdigit(peek())) {
        advance();
    }

    if (!is_at_end() && (peek() == '_' || std::isalpha(peek()))) {
        throw std::runtime_error("Invalid integer constant");
    }

    _tokens.push_back(Token(
                TokenType::constant, 
                _input.substr(start, _index - start)
                ));
}


void Lexer::lex_punctuation()
{
    switch(peek()) {
        case '(':
            _tokens.push_back(Token(TokenType::paren_open, "("));
            break;
        case ')':
            _tokens.push_back(Token(TokenType::paren_close, ")"));
            break;
        case '{':
            _tokens.push_back(Token(TokenType::brace_open, "{"));
            break;
        case '}':
            _tokens.push_back(Token(TokenType::brace_close, "}"));
            break;
        case ';':
            _tokens.push_back(Token(TokenType::semicolon, ";"));
            break;
        default:
            throw std::runtime_error("Not a valid punctuation");
    }

    advance();
}

void Lexer::skip_whitespace()
{
    while (!is_at_end() && is_whitespace(peek())) {
        advance();
    }
}


Lexer::Lexer(std::string input)
    : _input {input},
    _index {0}
{
    while (!is_at_end()) {
        skip_whitespace();

        if (is_at_end()) {
            std::cout << "at end now\n";
            break;
        }

        char curr = peek();
        try {
            if (std::isalpha(curr) || curr == '_') {
                lex_identifier_or_keyword();
            } else if (std::isdigit(curr)) {
                lex_constant();
            } else {
                lex_punctuation();
            }
        } catch (std::exception& e) {
            std::cerr << "Lexer Error: " << e.what() << std::endl;
        }
    }
}


std::string print_tok_type(TokenType t)
{
    switch (t) {
        case TokenType::identifier:
            return "IDENTIFIER";
        case TokenType::constant:
            return "CONSTANT";
        case TokenType::keyword_int:
            return "int";
        case TokenType::keyword_void:
            return "void";
        case TokenType::keyword_return:
            return "return";
        case TokenType::paren_open:
            return "(";
        case TokenType::paren_close:
            return ")";
        case TokenType::brace_open:
            return "{";
        case TokenType::brace_close:
            return "}";
        case TokenType::semicolon:
            return ";";
        default:
            return "";
    }
}


void Token::print()
{
    if (_token_type == TokenType::identifier || _token_type == TokenType::constant) {
        std::cout << print_tok_type(_token_type) << ": " << _value << std::endl;
    } else {
        std::cout << print_tok_type(_token_type) << std::endl;
    }
}


void Lexer::print()
{
    for (auto& t : _tokens) {
        t.print();
    }
}

