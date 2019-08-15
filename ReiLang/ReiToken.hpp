#ifndef REI_TOKEN
#define REI_TOKEN

#include <string>
#include <variant>
#include <iostream>

enum class TokenType
{
    eof,
    semi,
    variable,
    assign,
    plus,
    minus,
    mul,
    div,
    mod,
    l_par,
    r_par,
    integer,
    def_var,
    begin,
    end
};

inline std::string to_string(TokenType e)
{
    switch (e) {
    case TokenType::eof :
        return "eof";
    case TokenType::semi :
        return "semicolon";
    case TokenType::variable :
        return "variable";
    case TokenType::assign :
        return "assign";
    case TokenType::plus :
        return "plus";
    case TokenType::minus :
        return "minus";
    case TokenType::mul :
        return "mul";
    case TokenType::div :
        return "div";
    case TokenType::mod :
        return "mod";
    case TokenType::l_par :
        return "l_par";
    case TokenType::r_par :
        return "r_par";
    case TokenType::integer :
        return "integer";
    case TokenType::def_var :
        return "def_var";
    case TokenType::begin :
        return "begin";
    case TokenType::end :
        return "end";
    default :
        return "unknown";
    }
}

struct Token
{
    TokenType type;
    std::variant<int, std::string> data;
};

#endif // REI_TOKEN
