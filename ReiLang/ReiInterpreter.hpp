#ifndef REI_INTERPRETER
#define REI_INTERPRETER

#include "ReiParser.hpp"
#include <map>

class Interpreter
{
public:
    explicit Interpreter(std::string expression);
    void interpret();
private:

    void visit_node_(AstNode* n);
    BaseTypeVariable visit_expression_(AstNode* n);
    void visit_state_list_(StateListNode* n);
    void visit_def_list_(DefListNode* n);
    void visit_definition_(Definition* n, bool isConst);
    void visit_modification_(Modification* n);
    BaseTypeVariable visit_binary_operator_(BinaryOperator* n);
    BaseTypeVariable visit_unary_operator_(UnaryOperator* n);
    BaseTypeVariable visit_literal_(NumLiteral* n) const;
    BaseTypeVariable visit_identifier_(Identifier* n);

    void panic_(const std::string& msg) const;

    Parser parser_;
    std::map<std::string, BaseTypeVariable> global_scope_;
};

#endif // REI_INTERPRETER
