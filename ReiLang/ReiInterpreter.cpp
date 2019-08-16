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
        std::cout << p.first << " = ";
        print(p.second);
        std::cout << "\n";
    }
}

void Interpreter::visit_node_(AstNode* n)
{
    switch (n->type) {

    case AstNodeType::state_list: 
        visit_state_list_(dynamic_cast<StateListNode*>(n));
        break;
    case AstNodeType::def_list: 
        visit_def_list_(dynamic_cast<DefListNode*>(n));
        break;
    case AstNodeType::modification: 
        visit_modification_(dynamic_cast<Modification*>(n));
        break;
    default: 
        panic_("Bad parsing.");
    }
}

BaseTypeVariable Interpreter::visit_expression_(AstNode* n)
{
    if (!n) {
        return { BaseType::none };
    }
    switch (n->type) {
    case AstNodeType::binary_operator:
        return visit_binary_operator_(dynamic_cast<BinaryOperator*>(n));
    case AstNodeType::unary_operator:
        return visit_unary_operator_(dynamic_cast<UnaryOperator*>(n));
    case AstNodeType::variable:
        return visit_identifier_(dynamic_cast<Identifier*>(n));
    case AstNodeType::int_literal:
        return visit_literal_(dynamic_cast<NumLiteral*>(n));
    case AstNodeType::real_literal:
        return visit_literal_(dynamic_cast<NumLiteral*>(n));
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

void Interpreter::visit_def_list_(DefListNode* n)
{
    for (auto& d : n->definitions) {
        visit_definition_(dynamic_cast<Definition*>(d), n->isConstant);
    }
}


// 
// ToDo: I will refactor it later, u know, I will refactor it later
//
void Interpreter::visit_definition_(Definition* n, bool isConst)
{
    const std::string name = dynamic_cast<Identifier*>(n->variable)->name;
    const BaseType type = n->varType;
    if (global_scope_.find(name) != global_scope_.end()) {
        panic_("Redefinition of \"" + name + "\" identifier.");
    }
    BaseTypeVariable var = visit_expression_(n->expression);
    if (var.type == BaseType::none) {
        if (isConst) {
            panic_("Non-defined \"" + name + "\" constant.");
        }
        global_scope_[name] = BaseTypeVariable{ type };
    } else {
        if (type < var.type) {
            panic_("Implicit narrowing conversion in \"" + name + "\" definition");
        }
        if (type == BaseType::real) {
            var = casting::toTypeLocked(casting::toReal(var));
        }
        if (type == BaseType::integer) {
            var = casting::toTypeLocked(var);
        }
        if (isConst) {
            var = casting::toConst(var);
        }
        global_scope_[name] = var;
    }
}

void Interpreter::visit_modification_(Modification* n)
{
    const std::string name = dynamic_cast<Identifier*>(n->variable)->name;
    if (global_scope_.find(name) == global_scope_.end()) {
        panic_("Undefined identifier \"" + name + "\".");
    }
    if (global_scope_[name].isConst) {
        panic_("Assignation to const identifier \"" + name + "\".");
    }
    BaseTypeVariable var = visit_expression_(n->expression);
    if (!global_scope_[name].isTypeLocked) {
        global_scope_[name] = var;
    } else {
        if (global_scope_[name].type < var.type) {
            panic_("Implicit narrowing conversion in \"" + name + "\" assignment");
        }
        if (global_scope_[name].type == BaseType::real) {
            global_scope_[name] = casting::toTypeLocked(casting::toReal(var));
        } else {
            global_scope_[name] = casting::toTypeLocked(var);
        }
    }
}

BaseTypeVariable Interpreter::visit_binary_operator_(BinaryOperator* n)
{
    const BaseTypeVariable left  = visit_expression_(n->left);
    const BaseTypeVariable right = visit_expression_(n->right);
    switch (n->oper.type) {
    case TokenType::plus:
        return arithmetic::sum(left, right);
    case TokenType::minus: 
        return arithmetic::dif(left, right);
    case TokenType::mul: 
        return arithmetic::mul(left, right);
    case TokenType::div:
        return arithmetic::div(left, right);
    case TokenType::int_div: 
        return arithmetic::int_div(left, right);
    case TokenType::mod:
        return arithmetic::mod(left, right);
    default:
        panic_("Bad parsing.");
    }
}

BaseTypeVariable Interpreter::visit_unary_operator_(UnaryOperator* n)
{
    const BaseTypeVariable var = visit_expression_(n->operand);
    switch (n->oper.type) {
    case TokenType::plus:
        return var;
    case TokenType::minus:
        return arithmetic::neg(var);
    default:
        panic_("Bad parsing");
    }
}

BaseTypeVariable Interpreter::visit_literal_(NumLiteral* n) const
{
    if (n->type == AstNodeType::int_literal) {
        return { std::get<int>(n->data) };
    }
    return { std::get<double>(n->data) };
}

BaseTypeVariable Interpreter::visit_identifier_(Identifier* n)
{
    if (global_scope_.find(n->name) == global_scope_.end()) {
        panic_("Undefined identifier \"" + n->name + "\".");
    }
    if (global_scope_[n->name].type == BaseType::none) {
        panic_("Uninitialized identifier \"" + n->name + "\".");
    }
    if (global_scope_[n->name].type == BaseType::integer) {
        return { std::get<int>(global_scope_[n->name].value) };
    }
    return { std::get<double>(global_scope_[n->name].value) };
}

void Interpreter::panic_(const std::string& msg) const
{
    throw ReiException{ ReiExceptionType::interpreter_exception, msg };
}
