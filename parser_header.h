#ifndef PARSER_H_
#define PARSER_H_
#include <string>
#include <vector>
#include "lexer.h"
#ifdef DEBUG
	#define TRACE() do { std::cout << "ENTERING: " << __func__ << "\n"; } while(0)
#else
	#define TRACE() do {} while (0)
#endif

class Parser {
	Lexer lexer;
	void consume();
	void eat(TokenType expected_type);
	void match_terminal(std::string expected);
	void letter();
	void number();
	void stmt();
	void digit();
	void identifier();
	void program();
public:
	Token current_token;
	Parser(Lexer given_lexer);void parse();};
#endif // !PARSER_H_