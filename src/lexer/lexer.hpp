#include "token.hpp"
#include "../error/error.hpp"

#include <istream>
#include <optional>
#include <variant>

namespace compiler {

/* 
   Lexer takes a input stream and emits Tokens sequentially by 
   calling the member function GetNextToken
*/
using TokenOrError = std::variant<Token, Error>;

class Lexer {
    public:
        Lexer() = delete;
        Lexer(std::istream& source) : source_{source} {}
        ~Lexer() = default;

        TokenOrError GetNextToken();

    private:
        std::istream& source_;

        std::optional<Error> ConsumeWhile(std::string& token, bool (*predicate)(char));
        [[nodiscard]] TokenOrError LexIdentifierOrKeyword();
        [[nodiscard]] TokenOrError LexPunctuation();
        [[nodiscard]] TokenOrError LexConstant();
};

} // namespace compiler

