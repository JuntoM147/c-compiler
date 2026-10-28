#include <ostream>
#include <string>
#include <utility>

namespace compiler {

class Error
{
public:
    Error() = delete;
    Error(const std::string& err) : error_msg_ {err} {}
    Error(std::string&& err) : error_msg_ {std::move(err)} {}
    ~Error() = default;

    void PrintErr(std::ostream& out) const 
    {
        out << error_msg_ << std::endl;
    }

private:
    std::string error_msg_;
};

} // namespace compiler

