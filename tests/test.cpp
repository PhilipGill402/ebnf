#include <iostream>
#include <fstream>
#include "../include/lexer.h"
#include "../include/parser.h"
#include "../include/interpreter.h"

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
     
    file.close();

    Parser parser = Parser(lexer);
    std::cout << parser.current_token << "\n";

    std::unique_ptr<Syntax> root = parser.parse();

    Interpreter interpreter = Interpreter(std::move(root));
    interpreter.interpret();

    return 0; 
}
