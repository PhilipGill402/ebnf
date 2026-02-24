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
void identifier() {
letter();
while (current_token.lexeme == "a" || current_token.lexeme == "b" || current_token.lexeme == "c" || current_token.lexeme == "d" || current_token.lexeme == "e") {
letter();
}
}
void number() {
digit();
while (current_token.lexeme == "0" || current_token.lexeme == "1" || current_token.lexeme == "2" || current_token.lexeme == "3" || current_token.lexeme == "4") {
digit();
}
}
void letter() {
if (current_token.lexeme == "a") {
match_terminal("a");
}
else if (current_token.lexeme == "b") {
match_terminal("b");
}
else if (current_token.lexeme == "c") {
match_terminal("c");
}
else if (current_token.lexeme == "d") {
match_terminal("d");
}
else if (current_token.lexeme == "e") {
match_terminal("e");
}
}
void digit() {
if (current_token.lexeme == "0") {
match_terminal("0");
}
else if (current_token.lexeme == "1") {
match_terminal("1");
}
else if (current_token.lexeme == "2") {
match_terminal("2");
}
else if (current_token.lexeme == "3") {
match_terminal("3");
}
else if (current_token.lexeme == "4") {
match_terminal("4");
}
}
