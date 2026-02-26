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

std::unique_ptr<Syntax> Parser::syntax() {
    TRACE();
    
    while (current_token.type == TokenType::HASHTAG) {
        directive();         
    }

    return std::make_unique<Syntax>(std::move(parser_rules), std::move(lexer_rules), keywords, symbols);
}

void Parser::directive() {
    TRACE(); 
    
    while (current_token.type == TokenType::HASHTAG) {
        eat(TokenType::HASHTAG);

        if (current_token.lexeme == "lexer") {
            lexer_directive();
        } else if (current_token.lexeme == "parser") {
            parser_directive();
        } else {
            std::cerr << "expected either 'lexer' or 'parser' after '#', instead received '" << current_token.lexeme << "'\n";
            abort();
        }
    }

    if (current_token.lexeme == "parser") {
        parser_directive();
    }
}

void Parser::lexer_directive() {
    TRACE();

    eat(TokenType::NONTERMINAL);

    if (current_token.type == TokenType::HASHTAG) {
        eat(TokenType::HASHTAG);
        keywords.push_back(keyword());
    } else if (current_token.type == TokenType::NONTERMINAL){
        lexer_rules.push_back(syntax_rule());
    } else {
        std::cout << "expected '#' or a nonterminal, instead received '" << current_token.lexeme << "'\n";
        abort();
    }

    while (current_token.type == TokenType::HASHTAG || current_token.type == TokenType::NONTERMINAL) {
        if (current_token.type == TokenType::HASHTAG) {
            eat(TokenType::HASHTAG); 
            if (current_token.lexeme == "parser") {
                return;
            } else if (current_token.lexeme == "keyword") {
                keywords.push_back(keyword());
            } else if (current_token.lexeme == "symbol") {
                symbols.push_back(symbol());
            } else {
                std::cerr << "Unexpected keyword after '#', expected 'keyword', 'symbol', or 'parser' but received '" << current_token.lexeme << "'\n";
                abort();
            }
        } else {
            lexer_rules.push_back(syntax_rule());
        }
    }
}

void Parser::parser_directive() {
    TRACE();

    eat(TokenType::NONTERMINAL);

    parser_rules.push_back(syntax_rule());

    while (current_token.type == TokenType::NONTERMINAL) {
        parser_rules.push_back(syntax_rule());
    }
}

std::string Parser::keyword() {
    TRACE();

    eat(TokenType::NONTERMINAL); 
    std::string keyword = current_token.lexeme;
    eat(TokenType::NONTERMINAL);

    return keyword;
}

Symbol Parser::symbol() {
    eat(TokenType::NONTERMINAL);
    std::string type = current_token.lexeme;
    eat(TokenType::NONTERMINAL);
    std::string symbol = current_token.lexeme;
    current_token = lexer.get_next_token(); // dont want to eat because this could be any sort of symbol 

    return (Symbol){ .type = type, .symbol = symbol };
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

std::unique_ptr<Syntax> Parser::parse() {
    std::unique_ptr<Syntax> root = syntax();
    eat(TokenType::ENDOFFILE);

    return root;
}
