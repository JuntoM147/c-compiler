#include "lexer.hpp"

#include <iostream>
#include <string>
#include <utility>
#include <optional>

namespace compiler {

namespace {

constexpr int kEOF = std::char_traits<char>::eof();

// Assumes ASCII whitespace
bool IsWhiteSpace(char c)
{
    return c == '\t' || c == '\n' || c == '\v' || 
           c == '\f' || c == '\r' || c == ' ';
}

// True if c is [a-zA-Z] or _
bool IsAlphaOr_(char c) 
{
    return ('a' <= c && c <= 'z') || 
           ('A' <= c && c <= 'Z') || 
             c == '_';
}


bool IsDigit(char c) 
{
    return c >= '0' && c <= '9';
}

bool IsPunctuation(char c)
{
    return c == '(' || c == ')' || 
           c == '{' || c == '}' ||  
           c == ';';
}

bool IsIdentifierChar(char c) {
    return IsAlphaOr_(c) || IsDigit(c);
}

} // namespace


std::optional<Error> Lexer::ConsumeWhile(std::string& token, bool (*predicate)(char))
{
    int peek {source_.peek()};

    while (peek != kEOF && predicate(static_cast<char>(peek))) {
        int curr = source_.get();

        if (curr == kEOF) {
            return Error("Lexer Error: stream error");
        }

        // Guranteed not to be EOF
        token.push_back(static_cast<char>(curr)); 
        peek = source_.peek();
    }

    // Check that no stream error occurred
    if (peek == kEOF && (source_.bad() || !source_.eof())) {
        return Error("Lexer Error: stream error");
    }

    return {};
}


// Lexes [0-9]+
TokenOrError Lexer::LexConstant()
{
    std::string token {};

    auto err = ConsumeWhile(token, IsDigit);

    // Error
    if (err.has_value()) {
        return err.value();
    }

    int peek {source_.peek()};
    
    // Check the constant is not a invalid identifier (123abc)
    if (peek != kEOF && IsAlphaOr_(static_cast<char>(peek))) {
        return Error("Lexer Error: Encountered invalid identifier");
    }

    return Token(TokenType::kConstant, std::move(token));
}


TokenOrError Lexer::LexIdentifierOrKeyword()
{
    std::string token {};

    auto err = ConsumeWhile(token, IsIdentifierChar);

    // Error
    if (err.has_value()) {
        return err.value();
    }

    if (token == "int") {
        return Token(TokenType::kKeywordInt);
    } 

    if (token == "void") {
        return Token(TokenType::kKeywordVoid);
    } 

    if (token == "return") {
        return Token(TokenType::kKeywordReturn);
    } 

    return Token(TokenType::kIdentifier, std::move(token));
}


// Lexes '(' ')' '{' '}' ';' 
TokenOrError Lexer::LexPunctuation()
{
    char curr {static_cast<char>(source_.get())};
    switch(curr) {
        case '(':
            return Token(TokenType::kParenOpen);
        case ')':
            return Token(TokenType::kParenClose);
        case '{':
            return Token(TokenType::kBraceOpen);
        case '}':
            return Token(TokenType::kBraceClose);
        case ';':
            return Token(TokenType::kSemicolon);
        default:
            break;
    }

    std::string err_string = "Lexer Error: Encountered invalid punctuation: ";
    err_string += curr;
    return Error(std::move(err_string));
}


TokenOrError Lexer::GetNextToken()
{   
    // std::istream::peek() returns a int to represent all chars + eof (-1)
    int peek {source_.peek()};

    // Trim whitespace
    while (peek != kEOF && IsWhiteSpace(static_cast<char>(peek))) {
        source_.get();
        peek = source_.peek();
    }

    // Reached end of file or Stream error
    if (peek == kEOF) {
        if (source_.bad()) {
            return Error("Lexer Error: IO Stream Error (serious failure: bad bit set)");
        }

        if (source_.eof()) {
            return Token(TokenType::kEOF);
        }

        return Error("Lexer Error: IO stream error (other)");
    }

    // Valid character
    if (IsAlphaOr_(peek)) {
        return LexIdentifierOrKeyword();
    } 

    if (IsDigit(peek)) {
        return LexConstant();
    } 

    if (IsPunctuation(peek)) {
        return LexPunctuation();
    }

    std::string err_string = "Lexer Error: Unknown character: "; 
    err_string += static_cast<char>(peek);
    return Error(std::move(err_string));
}

} // namespace compiler

