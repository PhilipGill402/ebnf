#ifndef INCLUDE_PARSER_H
#define INCLUDE_PARSER_H

#include "lexer.h"
#include <iostream>

#ifdef DEBUG
    #define TRACE() do { std::cout << "ENTERING: " << __func__ << "\n"; } while(0)
#else
    #define TRACE() do {} while (0)
#endif


class Parser {
    Lexer lexer;
    
    void eat(TokenType expected_type);
    void syntax();
    void syntax_rule();
    void definitions_list();
    void single_definition();
    void syntactic_term();
    void syntactic_exception();
    void syntactic_factor();
    void syntactic_primary();
    void optional_sequence();
    void repeated_sequence();
    void grouped_sequence();
    void empty_sequence();
public:
    Token current_token;
    Parser(Lexer given_lexer);

    void parse(); 
};
#endif // !INCLUDE_PARSER_H
