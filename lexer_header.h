#ifndef LEXER_H_
#define LEXER_H_
#include <string>
typedef enum {
	SEMI,
	ASSIGN,
} TokenType;
typedef struct {
	TokenType type;
	std::string lexeme;
} Token;
class Lexer {
	std::string source;
	char curr_char
	int pos;
public:
	Lexer(std::string file);
	Token get_next_token();
#endif // !LEXER_H_