#ifndef REI_PARSER
#define REI_PARSER

#include "ReiLexer.hpp"
#include "ReiAst.hpp"

class Parser
{
public:
    explicit Parser(std::string expression);
    AstNode* parse();
private:
    AstNode* program_rule();
    AstNode* state_list_rule_();
    AstNode* state_rule_();
    AstNode* definition_rule_();
    AstNode* modification_rule_();
    AstNode* expression_rule_();
    AstNode* sum_rule_();
    AstNode* mult_rule_();
    AstNode* int_rule_();
    AstNode* var_rule_();
    void eat_(TokenType type);
    static void panic_(const std::string& msg);

    Lexer lexer_;
    Token curr_token_;
};

#endif // REI_PARSER
