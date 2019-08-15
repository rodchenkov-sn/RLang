#include "ReiLexer.hpp"
#include "ReiExcept.hpp"
#include <map>

std::map<std::string, TokenType> RESERVED_KEYWORDS = { {"var", TokenType::def_var} };

Lexer::Lexer(std::string expression) :
    expression_(std::move(expression)),
    pos_(0)
{
}

Token Lexer::getNextToken()
{
    skip_whitespaces_();
    if (pos_ < expression_.length() && expression_[pos_] == '~' && peek_() == '~') {
        skip_comment_();
    }
    if (pos_ >= expression_.length()) {
        return { TokenType::eof, 0 };
    }
    if (std::isdigit(expression_[pos_])) {
        return { TokenType::integer, {read_int_()} };
    }
    if (std::isalpha(expression_[pos_])) {
        return read_id_();
    }
    switch (expression_[pos_]) {
    case '+':
        ++pos_;
        return { TokenType::plus, 0 };
    case '-':
        ++pos_;
        return { TokenType::minus, 0 };
    case '*':
        ++pos_;
        return { TokenType::mul, 0 };
    case '/':
        ++pos_;
        return { TokenType::div, 0 };
    case '%':
        ++pos_;
        return { TokenType::mod, 0 };
    case '=':
        ++pos_;
        return { TokenType::assign, 0 };
    case '(':
        ++pos_;
        return { TokenType::l_par, 0 };
    case ')':
        ++pos_;
        return { TokenType::r_par, 0 };
    case '_':
        return read_id_();
    case ';':
        ++pos_;
        return { TokenType::semi, 0 };
    case '{':
        ++pos_;
        return { TokenType::begin, 0 };
    case '}':
        ++pos_;
        return { TokenType::end, 0 };
    default:
        panic_("Unknown lexeme");
    }
}

int Lexer::read_int_()
{
    std::string strInt;
    while (pos_ < expression_.length() && std::isdigit(expression_[pos_])) {
        strInt += expression_[pos_];
        ++pos_;
    }
    return std::stoi(strInt);
}

Token Lexer::read_id_()
{
    std::string id;
    while (pos_ < expression_.length() &&
        (std::isalpha(expression_[pos_]) || std::isdigit(expression_[pos_]) || expression_[pos_] == '_')) {
        id += expression_[pos_];
        ++pos_;
    }
    if (RESERVED_KEYWORDS.find(id) != RESERVED_KEYWORDS.end()) {
        return { RESERVED_KEYWORDS[id], 0 };
    }
    return { TokenType::variable, {id} };

}

char Lexer::peek_()
{
    if (pos_ + 1 < expression_.length()) {
        return expression_[pos_ + 1];
    }
    return '\0';
}

void Lexer::skip_whitespaces_()
{
    while (pos_ < expression_.length() && std::isspace(expression_[pos_])) {
        ++pos_;
    }
}

void Lexer::skip_comment_()
{
    while (pos_ < expression_.length() && expression_[pos_] != '\n') {
        ++pos_;
    }
    ++pos_;
}

void Lexer::panic_(std::string msg) const
{
    msg.append(" (\'");
    msg.append(1, expression_[pos_]);
    msg.append("\')");
    throw ReiException{ ReiExceptionType::lexer_exception, msg };
}
