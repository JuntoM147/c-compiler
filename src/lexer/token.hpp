#include <string>
#include <utility>

namespace compiler {

enum class TokenType {
    kIdentifier,
    kConstant,
    kKeywordInt,
    kKeywordVoid,
    kKeywordReturn,
    kParenOpen,
    kParenClose,
    kBraceOpen,
    kBraceClose,
    kSemicolon,
    kEOF
};


class Token {
    public:
        Token() = delete;
        Token(TokenType token_type, const std::string& token_value = "") 
            : token_type_ {token_type},
              value_ {token_value} {}
        Token(TokenType token_type, std::string&& token_value) 
            : token_type_ {token_type},
              value_ {std::move(token_value)} {}
        ~Token() = default;

    private:
        const TokenType token_type_;
        std::string value_;
};

} // namespace compiler

