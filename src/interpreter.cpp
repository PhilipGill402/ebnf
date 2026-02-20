#include "interpreter.h"

Interpreter::Interpreter(std::unique_ptr<Syntax> node): root(std::move(node)) {};

void Interpreter::visit_syntax(std::unique_ptr<Syntax> node) {
    VISIT_TRACE();

    for (int i = 0; i < node->rules.size(); i++) {
        visit_rule(std::move(node->rules[i]));
    }    
}

void Interpreter::visit_rule(std::unique_ptr<Rule> node) {
    VISIT_TRACE();

    visit_expr(std::move(node->expr));
}

void Interpreter::visit_expr(std::unique_ptr<Expr> node) {
    VISIT_TRACE();

    for (int i = 0; i < node->alternatives.size(); i++) {
        visit_sequence(std::move(node->alternatives[i]));
    }
}

void Interpreter::visit_sequence(std::unique_ptr<Sequence> node) {
    VISIT_TRACE();

    for (int i = 0; i < node->terms.size(); i++) {
         
    }
}

void Interpreter::visit_terminal(std::unique_ptr<Terminal> node) {
    VISIT_TRACE();
}

void Interpreter::visit_nonterminal(std::unique_ptr<Nonterminal> node) {
    VISIT_TRACE();
}

void Interpreter::visit_optional(std::unique_ptr<Optional> node) {
    VISIT_TRACE();
}

void Interpreter::visit_repeated(std::unique_ptr<Repeated> node) {
    VISIT_TRACE();
}

void Interpreter::visit_grouped(std::unique_ptr<Grouped> node) {
    VISIT_TRACE();
}

void Interpreter::visit_empty(std::unique_ptr<Empty> node) {
    VISIT_TRACE();
}

void Interpreter::interpret() {
    visit_syntax(std::move(root));
}
