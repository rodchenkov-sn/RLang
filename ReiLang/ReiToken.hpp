#ifndef REI_TOKEN
#define REI_TOKEN

#include <string>
#include <variant>
#include <iostream>

enum class TokenType
{
    none,
    eof,
    // eof
    semi,
    // ;
    comma,
    // ,
    identifier,
    // x
    assign,
    // =
    plus,
    // +
    minus,
    // -
    mul,
    // *
    div,
    // /
    int_div,
    // //
    mod,
    // %
    l_par,
    // (
    r_par,
    // )
    int_lit,
    // 123
    real_lit,
    // 1.23
    integer,
    // Int
    real,
    // Real
    def_var,
    // var
    def_con,
    // con
    begin,
    // {
    end,
    // }
    lock // ::
};

inline std::string to_string(TokenType e)
{
    switch (e) {
    case TokenType::eof :
        return "eof";
    case TokenType::semi :
        return "semi";
    case TokenType::comma :
        return "comma";
    case TokenType::identifier :
        return "identifier";
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
    case TokenType::int_div :
        return "int_div";
    case TokenType::mod :
        return "mod";
    case TokenType::l_par :
        return "l_par";
    case TokenType::r_par :
        return "r_par";
    case TokenType::int_lit :
        return "int_lit";
    case TokenType::integer :
        return "integer";
    case TokenType::real :
        return "real";
    case TokenType::def_var :
        return "def_var";
    case TokenType::def_con :
        return "def_con";
    case TokenType::begin :
        return "begin";
    case TokenType::end :
        return "end";
    case TokenType::lock :
        return "lock";
    case TokenType::real_lit: 
        return "real_lit";
    default :
        return "unknown";
    }
}

struct Token
{
    TokenType type;
    std::variant<int, double, std::string> data;
};


inline std::string to_string(Token t)
{
    if (t.type == TokenType::identifier) {
        return "(" + to_string(t.type) + " : " + std::get<std::string>(t.data) + ")";
    }
    if (t.type == TokenType::int_lit) {
        return  "(" + to_string(t.type) + " : " + std::to_string(std::get<int>(t.data)) + ")";
    }
    if (t.type == TokenType::real_lit) {
        return  "(" + to_string(t.type) + " : " + std::to_string(std::get<double>(t.data)) + ")";
    }
    return "(" + to_string(t.type) + ")";
}

#endif // REI_TOKEN
