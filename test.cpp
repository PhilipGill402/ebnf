void consume() {
	current_token = lexer.get_next_token();
}
void eat(TokenType expected_type) {
	#ifdef DEBUG
		std::cout << "Token: " << current_token << "\n";
	#endif // DEBUG
	if (expected_type != current_token.type) {
		std::cerr << "Expected: " << expected_type << ", Received: " << current_token.type << "\n";
		abort();
	}
	consume();
}
void match_terminal(std::string expected) {
	if (current_token.type != Token::TERMINAL || current_token.lexeme != expected) {
		std::cerr << "Expected: " << expected << ", Received " << current_token.lexeme << "\n";
		abort();
	}
	consume();
}
void program() {
while (current_token.lexeme == "let" || current_token.lexeme == "print") {
stmt();
match_terminal(";");
}
}
void stmt() {
if (current_token.lexeme == "let") {
eat(LET);
identifier();
eat(ASSGN);
number();
}
else if (current_token.lexeme == "print") {
eat(PRINT);
identifier();
}
}
