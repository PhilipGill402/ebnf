#include <iostream>
#include <fstream>
#include "../include/lexer.h"


int main() {
    std::ifstream file("file.ebnf");

    if (!file.is_open()) {
        std::cerr << "Could not open file 'file.ebnf'\n";
        return 1;
    }

    std::string contents;
    std::string line;
    while (std::getline(file, line)) {
        contents += line;
        contents += "\n";
    }

    Lexer lexer = Lexer(contents);
    Token curr_token = lexer.get_next_token();
    std::cout << curr_token << "\n";

    do {
        curr_token = lexer.get_next_token();
        std::cout << curr_token << "\n";
    } while (curr_token.type != TokenType::ENDOFFILE);
     
    file.close();

    return 0; 
}
