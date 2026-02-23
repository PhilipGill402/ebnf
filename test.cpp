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
if (current_token.lexeme == ";") {

}
void stmt() {
eat(LET);
eat(ASSGN);
eat(PRINT);
}
void identifier() {
}
void number() {
}
void letter() {
match_terminal("a");
match_terminal("b");
match_terminal("c");
match_terminal("d");
match_terminal("e");
}
void digit() {
match_terminal("0");
match_terminal("1");
match_terminal("2");
match_terminal("3");
match_terminal("4");
}
