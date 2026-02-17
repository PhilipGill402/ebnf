#include "parser.h" 

Parser::Parser(Lexer given_lexer): lexer(given_lexer) {
    current_token = lexer.get_next_token();
}

void Parser::eat(TokenType expected_type) {
    #ifdef DEBUG
        std::cout << "Token: " << current_token << "\n";                 
    #endif // DEBUG

    if (expected_type != current_token.type) {
        std::cerr << "Expected: " << expected_type << ", Received: " << current_token.type << "\n";
        abort();
    }

    current_token = lexer.get_next_token();
}

void Parser::syntax() {
    TRACE();
    
    syntax_rule();

    while (current_token.type == TokenType::NONTERMINAL) {
        syntax_rule();
    }
}

void Parser::syntax_rule() {
    TRACE();

    eat(TokenType::NONTERMINAL);
    eat(TokenType::EQUAL);

    definitions_list();

    eat(TokenType::SEMICOLON);
}

void Parser::definitions_list() {
    TRACE(); 

    single_definition();

    while (current_token.type == TokenType::PIPE) {
        eat(TokenType::PIPE);

        single_definition();
    }
}

void Parser::single_definition() {
    TRACE(); 

    syntactic_term();

    while (current_token.type == TokenType::COMMA) {
        eat(TokenType::COMMA);

        syntactic_term();
    }
}

void Parser::syntactic_term() {
    TRACE(); 

    syntactic_factor();

    if (current_token.type == TokenType::BAR) {
        eat(TokenType::BAR);

        syntactic_exception();
    }  
}

//TODO: implement exceptions 
void Parser::syntactic_exception() {
    TRACE(); 
    
    std::cerr << "Exceptions are currently not implemented. Please remove them from your ebnf file and retry\n";
    abort();

    return;
}

void Parser::syntactic_factor() {
    TRACE(); 

    if (current_token.type == INTEGER) {
        eat(INTEGER);
        eat(ASTERISK);
    }

    syntactic_primary();
}

void Parser::syntactic_primary() {
    TRACE(); 

    if (current_token.type == TokenType::LBRACKET) {
        optional_sequence(); 
    } else if (current_token.type == TokenType::LBRACE) {
        repeated_sequence(); 
    } else if (current_token.type == TokenType::LPAREN) {
        grouped_sequence(); 
    } else if (current_token.type == TokenType::NONTERMINAL) {
        eat(NONTERMINAL);
    } else if (current_token.type == TokenType::TERMINAL) {
        eat(TERMINAL);
    } else {
        empty_sequence();
    }
}

void Parser::optional_sequence() {
    TRACE(); 

    eat(LBRACKET);
    
    definitions_list();

    eat(RBRACKET);
}

void Parser::repeated_sequence() {
    TRACE(); 

    eat(LBRACE);

    definitions_list();

    eat(RBRACE);
}

void Parser::grouped_sequence() {
    TRACE(); 

    eat(LPAREN);

    definitions_list();

    eat(RPAREN);
}

void Parser::empty_sequence() {
    TRACE();
    TokenType type = current_token.type;

    if (type != TokenType::COMMA && type != PIPE && type != BAR && type != RBRACKET && type != RBRACE && type != RPAREN && type != SEMICOLON && type != ENDOFFILE) {
        std::cerr << "Unexpected token in 'syntactic_primary': " << current_token << "\n";
        abort();
    }
    
}

void Parser::parse() {
    syntax();
    eat(TokenType::ENDOFFILE);
}
