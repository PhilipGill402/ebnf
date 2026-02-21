#include "interpreter.h"
#include "visitor.h"

Interpreter::Interpreter(std::unique_ptr<Syntax> node): root(std::move(node)) {
    file.open("test.cpp");

    if (!file.is_open()) {
        std::cerr << "Error in creating file\n";
        abort();
    }
};

void Interpreter::visit(Syntax &node) {
    VISIT_TRACE();

    for (auto &rule : node.rules) {
        rule->accept(*this);
    }
}

void Interpreter::visit(Rule &node) {
    VISIT_TRACE();
    
    file << "void " << node.name << "() {\n";

    node.expr->accept(*this);

    file << "}\n";
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
    file << "void eat(TokenType expected_type) {\n" << "\t#ifdef DEBUG\n" << "\t\tstd::cout << \"Token: \" << current_token << \"\\n\";\n" << "\t#endif // DEBUG\n" << "\tif (expected_type != current_token.type) {\n";
    file << "\t\tstd::cerr << \"Expected: \" << expected_type << \", Received: \" << current_token.type << \"\\n\";\n" << "\t\t abort();\n" << "\t}\n" << "\tcurrent_token = lexer.get_next_token();\n" << "}\n";

    root->accept(*this); 
}
