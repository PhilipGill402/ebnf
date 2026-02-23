void eat(TokenType expected_type) {
	#ifdef DEBUG
		std::cout << "Token: " << current_token << "\n";
	#endif // DEBUG
	if (expected_type != current_token.type) {
		std::cerr << "Expected: " << expected_type << ", Received: " << current_token.type << "\n";
		 abort();
	}
	current_token = lexer.get_next_token();
}
void program() {
}
void stmt() {
}
void identifier() {
}
void number() {
}
void letter() {
}
void digit() {
}
