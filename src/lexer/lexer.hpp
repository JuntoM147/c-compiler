#include <string>
#include <vector>

enum class TokenType {
    identifier,
    constant,
    keyword_int,
    keyword_void,
    keyword_return,
    paren_open,
    paren_close,
    brace_open,
    brace_close,
    semicolon,
};

std::string print_tok_type(TokenType t);

class Token {
    public:
        Token() = delete;
        
        Token(TokenType token_type, std::string token_value) 
            : _token_type {token_type},
            _value {token_value}
        {}

        void print();

    private:
        TokenType _token_type;
        std::string _value;
};


class Lexer {
    public:
        Lexer() = delete;

        Lexer(std::string input);

        void print();

    private:
        bool is_at_end();
        char peek();
        void advance();
        char consume();
        void lex_identifier_or_keyword();
        void lex_constant();
        void lex_punctuation();
        void skip_whitespace();

        std::string _input;
        std::vector<Token> _tokens;
        size_t _index;
};

