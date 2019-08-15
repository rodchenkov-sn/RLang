#include "ReiExcept.hpp"
#include <utility>
#include <iostream>

ReiException::ReiException(const ReiExceptionType type, const std::string& message) :
    type_(type)
{
    switch (type_) {
    case ReiExceptionType::lexer_exception:
        message_ += "Lexing error: ";
        break;
    case ReiExceptionType::parser_exception:
        message_ += "Parsing error: ";
        break;
    case ReiExceptionType::interpreter_exception:
        message_ += "Interpreting error: ";
        break;
    }
    message_ += message;
}

[[nodiscard]] char const* ReiException::what() const
{
    return message_.c_str();
}
