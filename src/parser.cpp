#include "parser.h" 

Parser::Parser(Lexer given_lexer): lexer(std::move(given_lexer)) {
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

Syntax* Parser::syntax() {
    TRACE();
    
    std::vector<std::unique_ptr<Rule>> rules;

    rules.push_back(syntax_rule());

    while (current_token.type == TokenType::NONTERMINAL) {
        rules.push_back(syntax_rule());
    }

    return new Syntax(std::move(rules));
}

std::unique_ptr<Rule> Parser::syntax_rule() {
    TRACE();
    
    std::string name = current_token.lexeme;
    eat(TokenType::NONTERMINAL);
    eat(TokenType::EQUAL);

    std::unique_ptr<Expr> expr = definitions_list();

    eat(TokenType::SEMICOLON);

    std::unique_ptr<Rule> rule = std::make_unique<Rule>(name, std::move(expr));
    return rule;
}

std::unique_ptr<Expr> Parser::definitions_list() {
    TRACE(); 

    std::vector<std::unique_ptr<Sequence>> alternatives; 
    
    std::unique_ptr<Sequence> alternative = single_definition();
    alternatives.push_back(std::move(alternative));

    while (current_token.type == TokenType::PIPE) {
        eat(TokenType::PIPE);

        alternative = single_definition();
        alternatives.push_back(std::move(alternative));
    }
    
    std::unique_ptr<Expr> expr = std::make_unique<Expr>(std::move(alternatives));
    return expr;
}

std::unique_ptr<Sequence> Parser::single_definition() {
    TRACE(); 
    
    std::vector<std::unique_ptr<Term>> terms;

    std::unique_ptr<Term> term = syntactic_term();
    terms.push_back(std::move(term));

    while (current_token.type == TokenType::COMMA) {
        eat(TokenType::COMMA);

        term = syntactic_term();
        terms.push_back(std::move(term));
    }

    std::unique_ptr<Sequence> sequence = std::make_unique<Sequence>(std::move(terms));
    return sequence;
}

std::unique_ptr<Term> Parser::syntactic_term() {
    TRACE(); 
    
    std::unique_ptr<Term> term = syntactic_factor();
    
    //TODO: when exceptions are implemented add this to Term AST node
    if (current_token.type == TokenType::BAR) {
        eat(TokenType::BAR);

        syntactic_exception();
    } 

    return term;
}

//TODO: implement exceptions 
void Parser::syntactic_exception() {
    TRACE(); 
    
    std::cerr << "Exceptions are currently not implemented. Please remove them from your ebnf file and retry\n";
    abort();

    return;
}

std::unique_ptr<Term> Parser::syntactic_factor() {
    TRACE(); 
    
    //TODO: implement repeats
    if (current_token.type == TokenType::INTEGER) {
        std::cerr << "Repeats are currently not implemented. Please remove them from your ebnf file and retry\n";
        abort();
        eat(INTEGER);
        eat(ASTERISK);
    }

    std::unique_ptr<Term> term = syntactic_primary();
    return term;
}

std::unique_ptr<Term> Parser::syntactic_primary() {
    TRACE(); 
    
    std::unique_ptr<Term> term;
    if (current_token.type == TokenType::LBRACKET) {
        term = optional_sequence(); 
    } else if (current_token.type == TokenType::LBRACE) {
        term = repeated_sequence(); 
    } else if (current_token.type == TokenType::LPAREN) {
        term = grouped_sequence(); 
    } else if (current_token.type == TokenType::NONTERMINAL) {
        term = std::make_unique<Nonterminal>(current_token.lexeme);
        eat(NONTERMINAL);
    } else if (current_token.type == TokenType::TERMINAL) {
        term = std::make_unique<Terminal>(current_token.lexeme); 
        eat(TERMINAL);
    } else {
        term = empty_sequence();
    }

    return term;
}

std::unique_ptr<Optional> Parser::optional_sequence() {
    TRACE(); 

    eat(LBRACKET);
    
    std::unique_ptr<Expr> expr = definitions_list();

    eat(RBRACKET);

    return std::make_unique<Optional>(std::move(expr));
}

std::unique_ptr<Repeated> Parser::repeated_sequence() {
    TRACE(); 

    eat(LBRACE);

    std::unique_ptr<Expr> expr = definitions_list();

    eat(RBRACE);

    return std::make_unique<Repeated>(std::move(expr));
}

std::unique_ptr<Grouped> Parser::grouped_sequence() {
    TRACE(); 

    eat(LPAREN);

    std::unique_ptr<Expr> expr = definitions_list();

    eat(RPAREN);

    return std::make_unique<Grouped>(std::move(expr));
}

std::unique_ptr<Empty> Parser::empty_sequence() {
    TRACE();
    TokenType type = current_token.type;

    if (type != TokenType::COMMA && type != TokenType::PIPE && type != TokenType::BAR && type != TokenType::RBRACKET && type != TokenType::RBRACE && type != TokenType::RPAREN && type != TokenType::SEMICOLON && type != TokenType::ENDOFFILE) {
        std::cerr << "Unexpected token in 'syntactic_primary': " << current_token << "\n";
        abort();
    }

    return std::make_unique<Empty>();
    
}

Syntax* Parser::parse() {
    Syntax* root = syntax();
    eat(TokenType::ENDOFFILE);

    return root;
}
