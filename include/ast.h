#ifndef INCLUDE_AST_H_
#define INCLUDE_AST_H_

#include <vector>
#include <memory>

enum class NodeType {
    SYNTAX,
    RULE,
    EXPR,
    SEQUENCE,
    TERMINAL,
    NONTERMINAL,
    OPTIONAL,
    REPEATED,
    GROUPED,
    EMPTY
};

struct Rule;
struct Expr;
struct Sequence;
struct Term;
struct Visitor;

struct AST {
    NodeType type;

    AST(NodeType given_type);
    virtual ~AST() = default;

    AST(const AST&) = delete;
    AST& operator=(const AST&) = delete;
    AST(AST&&) = default;
    AST& operator=(AST&&) = default;
};

struct Syntax : AST {
    std::vector<std::unique_ptr<Rule>> rules;

    Syntax(std::vector<std::unique_ptr<Rule>> given_rules);
};

struct Rule : AST {
    std::string name;
    std::unique_ptr<Expr> expr; 

    Rule(std::string given_name, std::unique_ptr<Expr> given_expr);
};

struct Expr : AST {
    std::vector<std::unique_ptr<Sequence>> alternatives;

    Expr(std::vector<std::unique_ptr<Sequence>> given_alternatives);
};

struct Sequence : AST {
    std::vector<std::unique_ptr<Term>> terms;

    Sequence(std::vector<std::unique_ptr<Term>> given_terms);
};

struct Term : AST {
    Term(NodeType given_type);

    ~Term() override = default; // optional, but fine
};

struct Terminal : Term {
    std::string lexeme;

    Terminal(std::string given_lexeme);
};

struct Nonterminal : Term {
    std::string rule;

    Nonterminal(std::string given_rule);
};

struct Optional : Term {
    std::unique_ptr<Expr> expr;

    Optional(std::unique_ptr<Expr> given_expr);
};

struct Repeated : Term {
    std::unique_ptr<Expr> expr;

    Repeated(std::unique_ptr<Expr> given_expr);
};

struct Grouped : Term {
    std::unique_ptr<Expr> expr;

    Grouped(std::unique_ptr<Expr> given_expr);
};

struct Empty : Term {
    Empty();
};

#endif // !INCLUDE_ASH_H_
