#include "lexer.h"

std::ostream& operator<<(std::ostream& os, const Token& t) {
    os << "Token{type=" << t.type << ", lexeme=\"" << t.lexeme << "\"}";
    return os;
}

void Lexer::get_next_char() {
    if (pos > (int)source.size()) {
        curr_char = '\0'; 
        return;
    } 
    
    curr_char = source[pos++];
}

char Lexer::peek() {
    if (pos > (int)source.size()) {
        return '\0';
    } 

    return source[pos+1];
}

Lexer::Lexer(std::string file) {
    source = file;
    curr_char = source[0];
    pos = 0; 
    line = 0;
    col = 0;
}

std::string Lexer::terminal() {
    // assume all of this is inside quotes 
    std::stringstream ss; 
    while (curr_char != '"') {
        ss << curr_char;
        get_next_char();
    }
    // skip last quote
    get_next_char();

    return ss.str();
}

std::string Lexer::single_quote_terminal() {
    // assume all of this is inside single quotes
    std::stringstream ss; 
    while (curr_char != '\'') {
        ss << curr_char;
        get_next_char();
    }
    // skip last quote
    get_next_char();

    return ss.str();

}

std::string Lexer::nonterminal() {
    std::stringstream ss;
    while (std::isalnum(curr_char)) {
        ss << curr_char;
        get_next_char();
    }

    return ss.str();
}

Token Lexer::get_next_token() {
    while (curr_char != '\0') {
        if (curr_char == '(' && peek() == '*') {
            while (curr_char != '*' && peek() != ')') {
                get_next_char();
            }

            //skip last '*' and ')'
            get_next_char();
            get_next_char();

            continue;
        }

        if (std::isspace(curr_char)) {
            while (std::isspace(curr_char)) {
                get_next_char();
            }

            continue;
        }
        
        if (std::isalpha(curr_char)) {
            std::string lexeme = nonterminal();
            Token token = { .type = TokenType::NONTERMINAL, .lexeme = lexeme };

            return token;
        } else if (curr_char == '"') {
            get_next_char();
            std::string lexeme = terminal(); 
            Token token = { .type = TokenType::TERMINAL, .lexeme = lexeme };

            return token;
        } else if (curr_char == '\'') {
            get_next_char();
            std::string lexeme = single_quote_terminal();
            Token token = { .type = TokenType::TERMINAL, .lexeme = lexeme };

            return token;
        } else if (curr_char == '=') {
            Token token = { .type = TokenType::EQUAL, .lexeme = "=" };
            get_next_char();

            return token;
        } else if (curr_char == ',') {
            Token token = { .type = TokenType::COMMA, .lexeme = "," };
            get_next_char();

            return token;
        } else if (curr_char == ';') {
            Token token = { .type = TokenType::SEMICOLON, .lexeme = ";" };
            get_next_char();

            return token;
        } else if (curr_char == '|') {
            Token token = { .type = TokenType::PIPE, .lexeme = "|" };
            get_next_char();

            return token;
        } else if (curr_char == '[') {
            Token token = { .type = TokenType::LBRACKET, .lexeme = "[" };
            get_next_char();

            return token;
        } else if (curr_char == ']') {
            Token token = { .type = TokenType::RBRACKET, .lexeme = "]" };
            get_next_char();

            return token;
        } else if (curr_char == '{') {
            Token token = { .type = TokenType::LBRACE, .lexeme = "{" };
            get_next_char();

            return token;
        } else if (curr_char == '}') {
            Token token = { .type = TokenType::RBRACE, .lexeme = "}" };
            get_next_char();

            return token;
        } else if (curr_char == '(') {
            Token token = { .type = TokenType::LPAREN, .lexeme = "(" };
            get_next_char();

            return token;
        } else if (curr_char == ')') {
            Token token = { .type = TokenType::RPAREN, .lexeme = ")" };
            get_next_char();

            return token;
        } else if (curr_char == '*') {
            Token token = { .type = TokenType::ASTERISK, .lexeme = "*" };
            get_next_char();

            return token;
        } else if (curr_char == '-') {
            Token token = { .type = TokenType::BAR, .lexeme = "-" };
            get_next_char();

            return token;
        } else if (curr_char == '+') {
            Token token = { .type = TokenType::PLUS, .lexeme = "+" };
            get_next_char();

            return token;
        } else {
            // TODO: Implement actual ERROR handling
            std::cout << curr_char << "\n"; 
            throw std::runtime_error("Unexpected character found in source code");
        }
    } 
    
    Token token = { .type = ENDOFFILE, .lexeme = "END OF FILE"};
    return token;
}
