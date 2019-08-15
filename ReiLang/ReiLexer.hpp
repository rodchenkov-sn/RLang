#ifndef REI_LEXER
#define REI_LEXER

#include "ReiToken.hpp"

class Lexer final
{
public:
    explicit Lexer(std::string expression);
    Token getNextToken();
private:
    int read_int_();
    Token read_id_();
    char peek_();
    void skip_whitespaces_();
    void skip_comment_();
    void panic_(std::string msg) const;

    std::string expression_;
    size_t pos_;
};

#endif // REI_LEXER
