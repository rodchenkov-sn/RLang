#include "ReiParser.hpp"
#include <utility>
#include "ReiExcept.hpp"

Parser::Parser(std::string expression) :
    lexer_(std::move(expression)),
    curr_token_(lexer_.getNextToken())
{
}

AstNode* Parser::parse()
{
    return program_rule();
}

AstNode* Parser::program_rule()
{
    auto stateList = new StateListNode{};
    while (curr_token_.type != TokenType::eof) {
        stateList->states.push_back(state_rule_());
    }
    eat_(TokenType::eof);
    return stateList;
}

AstNode* Parser::def_list_rule_()
{
    DefListNode* defList = nullptr;
    if (curr_token_.type == TokenType::def_var) {
        defList = new DefListNode{ false };
        eat_(TokenType::def_var);
    } else {
        defList = new DefListNode{ true };
        eat_(TokenType::def_con);
    }
    while (curr_token_.type != TokenType::semi) {
        defList->definitions.push_back(definition_rule_());
        if (curr_token_.type == TokenType::comma) {
            eat_(TokenType::comma);
        }
    }
    return defList;
}

AstNode* Parser::state_list_rule_()
{
    auto stateList = new StateListNode{};
    while (curr_token_.type != TokenType::end) {
        stateList->states.push_back(state_rule_());
    }
    return stateList;
}

AstNode* Parser::state_rule_()
{
    if (curr_token_.type == TokenType::eof) {
        return nullptr;
    }
    if (curr_token_.type == TokenType::begin) {
        eat_(TokenType::begin);
        const auto stateList = state_list_rule_();
        eat_(TokenType::end);
        return stateList;
    }
    if (curr_token_.type == TokenType::def_var || curr_token_.type == TokenType::def_con) {
        const auto def = def_list_rule_();
        eat_(TokenType::semi);
        return def;
    }
    const auto mod = modification_rule_();
    eat_(TokenType::semi);
    return mod;
}

AstNode* Parser::definition_rule_()
{
    const auto identifier = identifier_rule_();
    BaseType type = BaseType::none;
    if (curr_token_.type == TokenType::lock) {
        eat_(TokenType::lock);
        if (curr_token_.type == TokenType::integer) {
            eat_(TokenType::integer);
            type = BaseType::integer;
        } else {
            eat_(TokenType::real);
            type = BaseType::real;
        }
    }
    Token oper = { TokenType::none, 0 };
    if (curr_token_.type == TokenType::assign) {
        oper = curr_token_;
        eat_(TokenType::assign);
        return new Definition{ identifier, expression_rule_(), oper, type };
    }
    return new Definition{ identifier, nullptr, oper, type };
}

AstNode* Parser::modification_rule_()
{
    const auto var = identifier_rule_();
    const Token op = curr_token_;
    eat_(TokenType::assign);
    return new Modification{ var, expression_rule_(), op };
}

AstNode* Parser::expression_rule_()
{
    return sum_rule_();
}

AstNode* Parser::sum_rule_()
{
    auto res = mult_rule_();
    AstNode* lowerNode = nullptr;
    while (curr_token_.type == TokenType::plus || curr_token_.type == TokenType::minus) {
        const Token op = curr_token_;
        if (op.type == TokenType::plus) {
            eat_(TokenType::plus);
        } else {
            eat_(TokenType::minus);
        }
        lowerNode = new BinaryOperator{ res, mult_rule_(), op };
        res = lowerNode;
    }
    return res;
}

AstNode* Parser::mult_rule_()
{
    auto res = liter_rule_();
    AstNode* lowerNode = nullptr;
    while (curr_token_.type == TokenType::mul 
        || curr_token_.type == TokenType::div 
        || curr_token_.type == TokenType::mod
        || curr_token_.type == TokenType::int_div) {
        const Token op = curr_token_;
        if (op.type == TokenType::mul) {
            eat_(TokenType::mul);
        } else if (op.type == TokenType::div) {
            eat_(TokenType::div);
        } else if(op.type == TokenType::int_div) {
            eat_(TokenType::int_div);
        } else {
            eat_(TokenType::mod);
        }
        lowerNode = new BinaryOperator{ res, liter_rule_(), op };
        res = lowerNode;
    }
    return res;
}

AstNode* Parser::liter_rule_()
{
    if (curr_token_.type == TokenType::plus) {
        eat_(TokenType::plus);
        return liter_rule_();
    }
    if (curr_token_.type == TokenType::minus) {
        Token op = curr_token_;
        eat_(TokenType::minus);
        return new UnaryOperator{ liter_rule_(), op };
    }
    if (curr_token_.type == TokenType::l_par) {
        eat_(TokenType::l_par);
        const auto res = expression_rule_();
        eat_(TokenType::r_par);
        return res;
    }
    if (curr_token_.type == TokenType::identifier) {
        return identifier_rule_();
    }
    if (curr_token_.type == TokenType::int_lit) {
        const Token intLit = curr_token_;
        eat_(TokenType::int_lit);
        return new NumLiteral{ std::get<int>(intLit.data) };
    }
    const Token intLit = curr_token_;
    eat_(TokenType::real_lit);
    return new NumLiteral{ std::get<double>(intLit.data) };
}

AstNode* Parser::identifier_rule_()
{
    const Token var = curr_token_;
    eat_(TokenType::identifier);
    return new Identifier{ std::get<std::string>(var.data) };
}

void Parser::eat_(const TokenType type)
{
    if (type == curr_token_.type) {
        curr_token_ = lexer_.getNextToken();
    } else {
        std::string msg = "Expected \"";
        msg += to_string(type);
        msg += "\" token, but got \"";
        msg += to_string(curr_token_.type);
        msg += "\" token.";
        panic_(msg);
    }
}

void Parser::panic_(const std::string& msg)
{
    throw ReiException{ ReiExceptionType::parser_exception, msg };
}
