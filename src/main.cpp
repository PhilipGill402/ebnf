#include <iostream>
#include <fstream>
#include "lexer.h"
#include "parser.h"
#include "interpreter.h"

int main(int argc, char** argv) {
    if (argc != 2) {
        std::cerr << "Usage: ebnf <input_file>\n";
        return 1;
    }

    std::ifstream file(argv[1]);

    if (!file.is_open()) {
        std::cerr << "Could not open file '" << argv[1] << "'\n";
        return 1;
    }

    std::string contents;
    std::string line;
    while (std::getline(file, line)) {
        contents += line;
        contents += "\n";
    }
    
    file.close();

    Lexer lexer = Lexer(contents);
    Parser parser = Parser(lexer);
    std::unique_ptr<Syntax> root = parser.parse();
    Interpreter interpreter = Interpreter(std::move(root));
    interpreter.interpret();
    

    return 0;
}
