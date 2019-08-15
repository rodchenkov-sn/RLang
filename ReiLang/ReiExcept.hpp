#ifndef REI_EXCEPT
#define REI_EXCEPT

#include <string>

enum class ReiExceptionType
{
    lexer_exception,
    parser_exception,
    interpreter_exception
};

class ReiException final : public std::exception
{
public:
    ReiException(ReiExceptionType type, const std::string& message);
    [[nodiscard]] char const* what() const override;
private:
    ReiExceptionType type_;
    std::string message_;
};

#endif // REI_EXCEPT
