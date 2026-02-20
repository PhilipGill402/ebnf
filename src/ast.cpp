#include "ast.h"

AST::AST(NodeType given_type): type(given_type) {};
Syntax::Syntax(std::vector<std::unique_ptr<Rule>> given_rules): AST(NodeType::SYNTAX), rules(std::move(given_rules)) {};
Rule::Rule(std::string given_name, std::unique_ptr<Expr> given_expr): AST(NodeType::RULE), name(given_name), expr(std::move(given_expr)) {};
Expr::Expr(std::vector<std::unique_ptr<Sequence>> given_alternatives): AST(NodeType::EXPR), alternatives(std::move(given_alternatives)) {};
Sequence::Sequence(std::vector<std::unique_ptr<Term>> given_terms): AST(NodeType::SEQUENCE), terms(std::move(given_terms)) {};
Term::Term(NodeType given_type): AST(given_type) {};
Terminal::Terminal(std::string given_lexeme): Term(NodeType::TERMINAL), lexeme(given_lexeme) {};
Nonterminal::Nonterminal(std::string given_rule): Term(NodeType::NONTERMINAL), rule(given_rule) {};
Optional::Optional(std::unique_ptr<Expr> given_expr): Term(NodeType::OPTIONAL), expr(std::move(given_expr)) {};
Repeated::Repeated(std::unique_ptr<Expr> given_expr): Term(NodeType::REPEATED), expr(std::move(given_expr)) {};
Grouped::Grouped(std::unique_ptr<Expr> given_expr): Term(NodeType::GROUPED), expr(std::move(given_expr)) {};
Empty::Empty(): Term(NodeType::EMPTY) {};

