#include "ReiInterpreter.hpp"
#include <utility>
#include "ReiExcept.hpp"

Interpreter::Interpreter(std::string expression) :
    parser_(std::move(expression))
{
}

void Interpreter::interpret()
{
    visit_node_(parser_.parse());
    for (auto& p : global_scope_) {
        std::cout << p.first << " = " << p.second << "\n";
    }
}

void Interpreter::visit_node_(AstNode* n)
{
    switch (n->type) {
    case AstNodeType::state_list:
        visit_state_list_(dynamic_cast<StateListNode*>(n));
        break;
    case AstNodeType::definition:
        visit_definition_(dynamic_cast<Definition*>(n));
        break;
    case AstNodeType::modification: 
        visit_modification_(dynamic_cast<Modification*>(n));
        break;
    default: 
        panic_("Bad parsing.");
    }
}

int Interpreter::visit_expression_(AstNode* n)
{
    switch (n->type) {
    case AstNodeType::binary_operator:
        return visit_binary_operator_(dynamic_cast<BinaryOperator*>(n));
    case AstNodeType::unary_operator:
        return visit_unary_operator_(dynamic_cast<UnaryOperator*>(n));
    case AstNodeType::int_literal:
        return visit_literal_(dynamic_cast<IntLiteral*>(n));
    case AstNodeType::variable:
        return visit_variable_(dynamic_cast<Variable*>(n));
    default:
        panic_("Bad parse.");
    }
}

void Interpreter::visit_state_list_(StateListNode* n)
{
    for (auto& s : n->states) {
        visit_node_(s);
    }
}

void Interpreter::visit_definition_(Definition* n)
{
    const std::string varName = dynamic_cast<Variable*>(n->variable)->name;
    if (global_scope_.find(varName) != global_scope_.end()) {
        panic_("Identifier redefinition (" + varName + ").");
    }
    global_scope_[varName] = visit_expression_(n->expression);
}

void Interpreter::visit_modification_(Modification* n)
{
    const std::string varName = dynamic_cast<Variable*>(n->variable)->name;
    if (global_scope_.find(varName) == global_scope_.end()) {
        panic_("Unknown identifier (" + varName + ").");
    }
    global_scope_[varName] = visit_expression_(n->expression);
}

int Interpreter::visit_binary_operator_(BinaryOperator* n)
{
    switch (n->oper.type) {
    case TokenType::plus:
        return visit_expression_(n->left) + visit_expression_(n->right);
    case TokenType::minus:
        return visit_expression_(n->left) - visit_expression_(n->right);
    case TokenType::mul:
        return visit_expression_(n->left) * visit_expression_(n->right);
    case TokenType::div:
        return visit_expression_(n->left) / visit_expression_(n->right);
    case TokenType::mod:
        return visit_expression_(n->left) % visit_expression_(n->right);
    default:
        panic_("Unexpected binary operator (" + to_string(n->oper.type) + ")");
    }
}

int Interpreter::visit_unary_operator_(UnaryOperator* n)
{
    switch (n->oper.type) {
    case TokenType::minus:
        return -1 * visit_expression_(n->operand);
    default:
        panic_("Unexpected unary operator (" + to_string(n->oper.type) + ")");
    }
}

int Interpreter::visit_literal_(IntLiteral* n)
{
    return n->value;
}

int Interpreter::visit_variable_(Variable* n)
{
    const std::string varName = n->name;
    if (global_scope_.find(varName) == global_scope_.end()) {
        panic_("Unknown identifier (" + varName + ").");
    }
    return global_scope_[varName];
}

void Interpreter::panic_(const std::string& msg) const
{
    throw ReiException{ ReiExceptionType::interpreter_exception, msg };
}
