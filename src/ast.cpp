#include "ast.h"
#include "visitor.h"

std::string node_type_name(NodeType t) {
    switch (t) {
        case NodeType::SYNTAX:      return "SYNTAX";
        case NodeType::RULE:        return "RULE";
        case NodeType::EXPR:        return "EXPR";
        case NodeType::SEQUENCE:    return "SEQUENCE";
        case NodeType::TERMINAL:    return "TERMINAL";
        case NodeType::NONTERMINAL: return "NONTERMINAL";
        case NodeType::OPTIONAL:    return "OPTIONAL";
        case NodeType::REPEATED:    return "REPEATED";
        case NodeType::GROUPED:     return "GROUPED";
        case NodeType::EMPTY:       return "EMPTY";
        default:                    return "UNKNOWN";
    }
}

AST::AST(NodeType given_type): type(given_type) {};
Syntax::Syntax(std::vector<std::unique_ptr<Rule>> given_rules): AST(NodeType::SYNTAX), rules(std::move(given_rules)) {};
void Syntax::accept(Visitor &v) {
    v.visit(*this);
}
Rule::Rule(std::string given_name, std::unique_ptr<Expr> given_expr): AST(NodeType::RULE), name(given_name), expr(std::move(given_expr)) {};
void Rule::accept(Visitor &v) {
    v.visit(*this);
}
Expr::Expr(std::vector<std::unique_ptr<Sequence>> given_alternatives): AST(NodeType::EXPR), alternatives(std::move(given_alternatives)) {};
void Expr::accept(Visitor &v) {
    v.visit(*this);
}
Sequence::Sequence(std::vector<std::unique_ptr<Term>> given_terms): AST(NodeType::SEQUENCE), terms(std::move(given_terms)) {};
void Sequence::accept(Visitor &v) {
    v.visit(*this);
}
Term::Term(NodeType given_type): AST(given_type) {};
Terminal::Terminal(std::string given_lexeme): Term(NodeType::TERMINAL), lexeme(given_lexeme) {};
void Terminal::accept(Visitor &v) {
    v.visit(*this);
}
Nonterminal::Nonterminal(std::string given_rule): Term(NodeType::NONTERMINAL), rule(given_rule) {};
void Nonterminal::accept(Visitor &v) {
    v.visit(*this); 
}
Optional::Optional(std::unique_ptr<Expr> given_expr): Term(NodeType::OPTIONAL), expr(std::move(given_expr)) {};
void Optional::accept(Visitor &v) {
    v.visit(*this);
}
Repeated::Repeated(std::unique_ptr<Expr> given_expr): Term(NodeType::REPEATED), expr(std::move(given_expr)) {};
void Repeated::accept(Visitor &v) {
    v.visit(*this);
}
Grouped::Grouped(std::unique_ptr<Expr> given_expr): Term(NodeType::GROUPED), expr(std::move(given_expr)) {};
void Grouped::accept(Visitor &v) {
    v.visit(*this);
}
Empty::Empty(): Term(NodeType::EMPTY) {};
void Empty::accept(Visitor &v) {
    v.visit(*this);
}
