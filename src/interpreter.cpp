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
    file << "void eat(TokenType expected_type) {\n";
    file << "\t#ifdef DEBUG\n";
    file << "\t\tstd::cout << \"Token: \" << current_token << \"\\n\";\n";
    file << "\t#endif // DEBUG\n";
    file << "\tif (expected_type != current_token.type) {\n";
    file << "\t\tstd::cerr << \"Expected: \" << expected_type << \", Received: \" << current_token.type << \"\\n\";\n";
    file << "\t\t abort();\n";
    file << "\t}\n";
    file << "\tcurrent_token = lexer.get_next_token();\n";
    file << "}\n";

    root->accept(*this); 
}

TerminalFinder::TerminalFinder(std::unique_ptr<Syntax> node): root(std::move(node)) {}

void TerminalFinder::visit(Syntax &node) {
    for (auto &rule : node.rules) {
        rule->accept(*this);
    }
}

void TerminalFinder::visit(Rule &node) {
    rules[node.name] = &node;
    node.expr->accept(*this);
}

void TerminalFinder::visit(Expr &node) {
    for (auto &alternative : node.alternatives) {
        can_append = true;
        alternative->accept(*this); 
    }
}

void TerminalFinder::visit(Sequence &node) {
    node.terms[0]->accept(*this);
}

void TerminalFinder::visit(Terminal &node) {
    if (can_append) {
        terminals.push_back(node.lexeme);
        can_append = false;
    }
}

void TerminalFinder::visit(Nonterminal &node) {
    std::string rule = node.rule;  
}

void TerminalFinder::visit(Optional &node) {
    node.expr->accept(*this);
}

void TerminalFinder::visit(Repeated &node) {
    node.expr->accept(*this);
}

void TerminalFinder::visit(Grouped &node) {
    node.expr->accept(*this);
}

void TerminalFinder::visit(Empty &node) {
    return;
}

std::vector<std::string> TerminalFinder::find_first_terminals() {
    
}

