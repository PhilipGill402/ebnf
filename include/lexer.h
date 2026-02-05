#ifndef INCLUDE_LEXER_H_
#define INCLUDE_LEXER_H_

#include <sstream>
#include <iostream>
#include <string>

typedef enum {
    NONTERMINAL,
    TERMINAL,
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
    QUOTE,
    ASTERISK,
    BAR,
    ENDOFFILE
} TokenType;

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

    char get_next_char();
    char peek();
    std::string id();

public:
    Lexer(std::string file);
    Token get_next_token();
};

#endif // !INCLUDE_LEXER_H_


