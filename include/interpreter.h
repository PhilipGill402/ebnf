#ifndef INCLUDE_INTERPRETER_H_
#define INCLUDE_INTERPRETER_H_

#include <iostream>
#include <memory.h>
#include "ast.h"

#ifdef DEBUG
    #define VISIT_TRACE() do { std::cout << "VISITING: " << __func__ << "\n"; } while(0)
#else
    #define VISIT_TRACE() do {} while (0)
#endif

class Interpreter {
    std::unique_ptr<Syntax> root;

    void visit_syntax(std::unique_ptr<Syntax> node);
    void visit_rule(std::unique_ptr<Rule> node);
    void visit_expr(std::unique_ptr<Expr> node);
    void visit_sequence(std::unique_ptr<Sequence> node);
    void visit_terminal(std::unique_ptr<Terminal> node);
    void visit_nonterminal(std::unique_ptr<Nonterminal> node);
    void visit_optional(std::unique_ptr<Optional> node);
    void visit_repeated(std::unique_ptr<Repeated> node);
    void visit_grouped(std::unique_ptr<Grouped> node);
    void visit_empty(std::unique_ptr<Empty> node);

public:
    Interpreter(std::unique_ptr<Syntax> node);

    void interpret();
};

#endif // !INCLUDE_INTERPRETER_H_
