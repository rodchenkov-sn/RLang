#include "ReiAst.hpp"

AstNode::AstNode(const AstNodeType t): type(t)
{
}

StateListNode::StateListNode(): AstNode(AstNodeType::state_list)
{
}

StateListNode::~StateListNode()
{
    for (auto& s : states)
        delete s;
}

DefListNode::DefListNode(const bool isCon):
    AstNode(AstNodeType::def_list),
    isConstant(isCon)
{
}

DefListNode::~DefListNode()
{
    for (auto& d : definitions) {
        delete d;
    }
}

Definition::Definition(AstNode* var, AstNode* exp, Token t, BaseType tp):
    AstNode(AstNodeType::definition),
    variable(var),
    expression(exp),
    oper(std::move(t)),
    varType(tp)
{
}

Definition::~Definition()
{
    delete variable;
    delete expression;
}

Modification::Modification(AstNode* var, AstNode* exp, Token t):
    AstNode(AstNodeType::modification),
    variable(var),
    expression(exp),
    oper(std::move(t))
{
}

Modification::~Modification()
{
    delete variable;
    delete expression;
}

BinaryOperator::BinaryOperator(AstNode* l, AstNode* r, Token t):
    AstNode(AstNodeType::binary_operator),
    left(l),
    right(r),
    oper(std::move(t))
{
}

BinaryOperator::~BinaryOperator()
{
    delete left;
    delete right;
}

UnaryOperator::UnaryOperator(AstNode* n, Token t):
    AstNode(AstNodeType::unary_operator),
    operand(n),
    oper(std::move(t))
{
}

UnaryOperator::~UnaryOperator()
{
    delete operand;
}

Identifier::Identifier(std::string n):
    AstNode(AstNodeType::variable),
    name(std::move(n))
{
}

NumLiteral::NumLiteral(const int v):
    AstNode(AstNodeType::int_literal),
    data(v)
{
}

NumLiteral::NumLiteral(const double v):
    AstNode(AstNodeType::real_literal),
    data(v)
{
}
