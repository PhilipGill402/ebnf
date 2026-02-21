#ifndef INCLUDE_INTERPRETER_H_
#define INCLUDE_INTERPRETER_H_

#include <iostream>
#include <memory.h>
#include "ast.h"
#include "visitor.h"

#ifdef DEBUG
    #define VISIT_TRACE() do { std::cout << "VISITING: " << " [" << node_type_name((node).type) << "]\n"; } while(0)
#else
    #define VISIT_TRACE() do {} while (0)
#endif

class Interpreter : Visitor {
    std::unique_ptr<Syntax> root;
    
    void visit(Syntax &node);
    void visit(Rule &node);
    void visit(Expr &node);
    void visit(Sequence &node);
    void visit(Terminal &node);
    void visit(Nonterminal &node);
    void visit(Optional &node);
    void visit(Repeated &node);
    void visit(Grouped &node);
    void visit(Empty &node);

public:
    Interpreter(std::unique_ptr<Syntax> node);

    void interpret();
};

#endif // !INCLUDE_INTERPRETER_H_
