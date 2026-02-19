#include "ast.h"

Syntax::Syntax(std::vector<std::unique_ptr<Rule>> given_rules): rules(std::move(given_rules)) {};
Rule::Rule(std::string given_name, std::unique_ptr<Expr> given_expr): name(given_name), expr(std::move(given_expr)) {};
Expr::Expr(std::vector<std::unique_ptr<Sequence>> given_alternatives): alternatives(std::move(given_alternatives)) {};
Sequence::Sequence(std::vector<std::unique_ptr<Term>> given_terms): terms(std::move(given_terms)) {};
Terminal::Terminal(std::string given_lexeme): lexeme(given_lexeme) {};
Nonterminal::Nonterminal(std::string given_rule): rule(given_rule) {};
Optional::Optional(std::unique_ptr<Expr> given_expr): expr(std::move(given_expr)) {};
Repeated::Repeated(std::unique_ptr<Expr> given_expr): expr(std::move(given_expr)) {};
Grouped::Grouped(std::unique_ptr<Expr> given_expr): expr(std::move(given_expr)) {};
Empty::Empty() {};

