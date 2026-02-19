#ifndef INCLUDE_PARSER_H
#define INCLUDE_PARSER_H

#include "lexer.h"
#include "ast.h"
#include <iostream>
#include <memory>

#ifdef DEBUG
    #define TRACE() do { std::cout << "ENTERING: " << __func__ << "\n"; } while(0)
#else
    #define TRACE() do {} while (0)
#endif


class Parser {
    Lexer lexer;
    
    void eat(TokenType expected_type);
    Syntax* syntax();
    std::unique_ptr<Rule> syntax_rule();
    std::unique_ptr<Expr> definitions_list();
    std::unique_ptr<Sequence> single_definition();
    std::unique_ptr<Term> syntactic_term();
    void syntactic_exception();
    std::unique_ptr<Term> syntactic_factor();
    std::unique_ptr<Term> syntactic_primary();
    std::unique_ptr<Optional> optional_sequence();
    std::unique_ptr<Repeated> repeated_sequence();
    std::unique_ptr<Grouped> grouped_sequence();
    std::unique_ptr<Empty> empty_sequence();
public:
    Token current_token;
    Parser(Lexer given_lexer);

    Syntax* parse(); 
};
#endif // !INCLUDE_PARSER_H
