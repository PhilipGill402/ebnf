#ifndef INCLUDE_AST_H_
#define INCLUDE_AST_H_

#include <vector>
#include <memory>
#include <string>

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

std::string node_type_name(NodeType t);
    
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

    virtual void accept(Visitor &v) = 0;
};

struct Syntax : AST {
    std::vector<std::unique_ptr<Rule>> rules;

    Syntax(std::vector<std::unique_ptr<Rule>> given_rules);
    void accept(Visitor &n) override;
};

struct Rule : AST {
    std::string name;
    std::unique_ptr<Expr> expr; 

    Rule(std::string given_name, std::unique_ptr<Expr> given_expr);
    void accept(Visitor &n) override;
};

struct Expr : AST {
    std::vector<std::unique_ptr<Sequence>> alternatives;

    Expr(std::vector<std::unique_ptr<Sequence>> given_alternatives);
    void accept(Visitor &n) override;
};

struct Sequence : AST {
    std::vector<std::unique_ptr<Term>> terms;

    Sequence(std::vector<std::unique_ptr<Term>> given_terms);
    void accept(Visitor &n) override;
};

struct Term : AST {
    Term(NodeType given_type);
    ~Term() override = default;
};

struct Terminal : Term {
    std::string lexeme;

    Terminal(std::string given_lexeme);
    void accept(Visitor &v) override;
};

struct Nonterminal : Term {
    std::string rule;

    Nonterminal(std::string given_rule);
    void accept(Visitor &v) override;
};

struct Optional : Term {
    std::unique_ptr<Expr> expr;

    Optional(std::unique_ptr<Expr> given_expr);
    void accept(Visitor &v) override;
};

struct Repeated : Term {
    std::unique_ptr<Expr> expr;

    Repeated(std::unique_ptr<Expr> given_expr);
    void accept(Visitor &v) override;
};

struct Grouped : Term {
    std::unique_ptr<Expr> expr;

    Grouped(std::unique_ptr<Expr> given_expr);
    void accept(Visitor &v) override;
};

struct Empty : Term {
    Empty();
    void accept(Visitor &v) override;
};

#endif // !INCLUDE_ASH_H_
