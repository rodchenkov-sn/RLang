#include "ReiLexer.hpp"
#include "ReiExcept.hpp"
#include <map>

std::map<std::string, TokenType> RESERVED_KEYWORDS = { 
    {"var", TokenType::def_var},
    {"con", TokenType::def_con},
    {"Int", TokenType::integer},
    {"Real", TokenType::real}
};

Lexer::Lexer(std::string expression) :
    expression_(std::move(expression)),
    pos_(0)
{
}

Token Lexer::getNextToken()
{
    while (true) {
        if (pos_ < expression_.length() && expression_[pos_] == '~' && peek_() == '~') {
            pos_ += 2;
            skip_comment_();
        }
        skip_whitespaces_();
        if (pos_ >= expression_.length()) {
            return { TokenType::eof, 0 };
        }
        if (std::isdigit(expression_[pos_])) {
            return read_num_literal_();
        }
        if (std::isalpha(expression_[pos_])) {
            return read_id_();
        }
        if (expression_[pos_] == ':' && peek_() == ':') {
            pos_ += 2;
            return { TokenType::lock , {0} };
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
            if (peek_() == '/') {
                pos_ += 2;
                return { TokenType::int_div, 0 };
            }
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
        case ',':
            ++pos_;
            return { TokenType::comma, 0 };
        case '~':
            continue;
        default:
            panic_("Unknown lexeme");
        }
    }
}

Token Lexer::read_num_literal_()
{
    TokenType type = TokenType::int_lit;
    std::string strLiteral;
    while (pos_ < expression_.length() && (std::isdigit(expression_[pos_]) || expression_[pos_] == '.')) {
        if (expression_[pos_] == '.') {
            if (type == TokenType::real_lit) {
                panic_("Bad Real literal");
            }
            type = TokenType::real_lit;
        }
        strLiteral += expression_[pos_];
        ++pos_;
    }
    if (type == TokenType::real_lit) {
        return { type, std::stod(strLiteral) };
    }
    return { type, std::stoi(strLiteral) };
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
    return { TokenType::identifier, {id} };

}

char Lexer::peek_(size_t l)
{
    if (pos_ + l < expression_.length()) {
        return expression_[pos_ + l];
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
    if (pos_ < expression_.length() && expression_[pos_] == '[') {
        while (pos_ < expression_.length()) {
            if (expression_[pos_] == ']' && peek_() == '~' && peek_(2) == '~') {
                break;
            }
            ++pos_;
        }
        pos_ += 3;
    } else {
        while (pos_ < expression_.length() && expression_[pos_] != '\n') {
            ++pos_;
        }
    }
}

void Lexer::panic_(std::string msg) const
{
    msg.append(" (\'");
    msg.append(1, expression_[pos_]);
    msg.append("\')");
    throw ReiException{ ReiExceptionType::lexer_exception, msg };
}
