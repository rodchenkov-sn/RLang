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
    int visit_expression_(AstNode* n);
    void visit_state_list_(StateListNode* n);
    void visit_definition_(Definition* n);
    void visit_modification_(Modification* n);
    int visit_binary_operator_(BinaryOperator* n);
    int visit_unary_operator_(UnaryOperator* n);
    int visit_literal_(IntLiteral* n);
    int visit_variable_(Variable* n);

    void panic_(const std::string& msg) const;

    Parser parser_;
    std::map<std::string, int> global_scope_;
};

#endif // REI_INTERPRETER
