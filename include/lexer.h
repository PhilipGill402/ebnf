#ifndef INCLUDE_LEXER_H_
#define INCLUDE_LEXER_H_

#include <sstream>
#include <iostream>
#include <string>

typedef enum {
    NONTERMINAL,
    TERMINAL,
    INTEGER,
    EQUAL,
    COMMA,
    SEMICOLON,
    PIPE,
    LBRACKET,
    RBRACKET,
    LBRACE,
    RBRACE,
    LPAREN,
    RPAREN,
    ASTERISK,
    BAR,
    PLUS,
    ENDOFFILE
} TokenType;

std::ostream& operator<<(std::ostream& os, TokenType t);

typedef struct {
    TokenType type;
    std::string lexeme;
} Token;

std::ostream& operator<<(std::ostream& os, const Token& t);

class Lexer {
    std::string source;
    char curr_char;
    int pos; 
    int line;
    int col;

    void get_next_char();
    char peek();
    std::string terminal();
    std::string single_quote_terminal();
    std::string nonterminal();
    std::string integer();

public:
    Lexer(std::string file);
    Token get_next_token();
};

#endif // !INCLUDE_LEXER_H_


