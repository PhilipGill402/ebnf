#include "main.h"

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

    Lexer lexer = Lexer(contents);
    
    file.close();

    return 0;
}
