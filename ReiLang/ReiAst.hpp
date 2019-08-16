#ifndef REI_AST
#define REI_AST

#include <list>
#include <string>
#include "ReiToken.hpp"
#include "ReiBaseTypes.hpp"

enum class AstNodeType
{
    state_list,
    def_list,
    definition,
    modification,
    variable,
    binary_operator,
    unary_operator,
    int_literal,
    real_literal,
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

struct DefListNode final : AstNode
{
    explicit DefListNode(bool isCon);
    ~DefListNode();
    std::list<AstNode*> definitions;
    bool isConstant;
};

struct Definition final : AstNode
{
    Definition(AstNode* var, AstNode* exp, Token t, BaseType tp);
    ~Definition();
    AstNode* variable;
    AstNode* expression;
    Token oper;
    BaseType varType;
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

struct Identifier final : AstNode
{
    explicit Identifier(std::string n);
    std::string name;
};

struct NumLiteral final : AstNode
{
    explicit NumLiteral(int v);
    explicit NumLiteral(double v);
    std::variant<int, double> data;
};

#endif // REI_AST
