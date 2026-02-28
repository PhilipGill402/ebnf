#ifndef INCLUDE_INTERPRETER_H_
#define INCLUDE_INTERPRETER_H_

#include <iostream>
#include <memory>
#include <fstream>
#include <unordered_map>
#include <vector>
#include <string>
#include "ast.h"
#include "visitor.h"

#ifdef DEBUG
    #define VISIT_TRACE() do { std::cout << "VISITING: " << "[" << node_type_name((node).type) << "]\n"; } while(0)
#else
    #define VISIT_TRACE() do {} while (0)
#endif

class Interpreter {
    std::unique_ptr<Syntax> root;

public:
    Interpreter(std::unique_ptr<Syntax> node);

    void interpret();
};

class ParserInterpreter : Visitor {
    Syntax &root;
    std::unordered_map<std::string, Rule*> rules;
    std::ofstream file;
    std::ofstream header;
    
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

    void create_rule_table();
    void generate_header();

public:
    ParserInterpreter(Syntax &node);

    void generate();
};

class TerminalFinder : Visitor {
    Sequence &root;
    std::unordered_map<std::string, Rule*> rules;
    std::vector<std::string> terminals;
    bool can_append;

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
    TerminalFinder(Sequence &node, std::unordered_map<std::string, Rule*> &given_rules);
    std::vector<std::string> find_first_terminals();
};

#endif // !INCLUDE_INTERPRETER_H_
