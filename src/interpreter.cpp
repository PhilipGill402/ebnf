#include "interpreter.h"
#include "visitor.h"

Interpreter::Interpreter(std::unique_ptr<Syntax> node): root(std::move(node)) {};

void Interpreter::visit(Syntax &node) {
    VISIT_TRACE();

    for (auto &rule : node.rules) {
        rule->accept(*this);
    }
}

void Interpreter::visit(Rule &node) {
    VISIT_TRACE();
    
    node.expr->accept(*this);
}

void Interpreter::visit(Expr &node) {
    VISIT_TRACE();

    for (auto &alternative : node.alternatives) {
        alternative->accept(*this); 
    }
}

void Interpreter::visit(Sequence &node) {
    VISIT_TRACE();

    for (auto &term : node.terms) {
        term->accept(*this); 
    }
}

void Interpreter::visit(Terminal &node) {
    VISIT_TRACE();
}
void Interpreter::visit(Nonterminal &node) {
    VISIT_TRACE();
}

void Interpreter::visit(Optional &node) {
    VISIT_TRACE();
}

void Interpreter::visit(Repeated &node) {
    VISIT_TRACE();
}

void Interpreter::visit(Grouped &node) {
    VISIT_TRACE();
}

void Interpreter::visit(Empty &node) {
    VISIT_TRACE();
}

void Interpreter::interpret() {
    root->accept(*this); 
}
