#ifndef REI_AST
#define REI_AST

#include <list>
#include <string>
#include <utility>
#include "ReiToken.hpp"

enum class AstNodeType
{
    state_list,
    definition,
    modification,
    variable,
    binary_operator,
    unary_operator,
    int_literal,
    none
};

struct AstNode
{
    explicit AstNode(AstNodeType t);
    virtual ~AstNode() = default;
    AstNode(const AstNode&)              = delete;
    AstNode(AstNode&&)                   = delete;
    AstNode& operator = (const AstNode&) = delete;
    AstNode& operator = (AstNode&&)      = delete;
    AstNodeType type;
};

struct StateListNode final : AstNode
{
    StateListNode();
    ~StateListNode();
    std::list<AstNode*> states;
};

struct Definition final : AstNode
{
    Definition(AstNode* var, AstNode* exp, Token t);
    ~Definition();
    AstNode* variable;
    AstNode* expression;
    Token oper;
};

struct Modification final : AstNode
{
    Modification(AstNode* var, AstNode* exp, Token t);
    ~Modification();
    AstNode* variable;
    AstNode* expression;
    Token oper;
};

struct BinaryOperator final : AstNode
{
    BinaryOperator(AstNode* l, AstNode* r, Token t);
    ~BinaryOperator();
    AstNode* left;
    AstNode* right;
    Token oper;
};

struct UnaryOperator final : AstNode
{
    UnaryOperator(AstNode* n, Token t);
    ~UnaryOperator();
    AstNode* operand;
    Token oper;
};

struct Variable final : AstNode
{
    explicit Variable(std::string n);
    std::string name;
};

struct IntLiteral final : AstNode
{
    explicit IntLiteral(int v);
    int value;
};

#endif // REI_AST
