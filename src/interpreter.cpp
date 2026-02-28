#include "interpreter.h"
#include "visitor.h"

std::string token_enum_for_terminal(const std::string& lex) {
    // punctuation / operators
    if (lex == "=") return "ASSGN";
    if (lex == "+") return "ADDOP";
    if (lex == "-") return "SUBOP";
    if (lex == "*") return "MULTOP";
    if (lex == "/") return "DIVOP";
    if (lex == "<") return "LT";
    if (lex == ">") return "GT";
    if (lex == "&") return "ANDOP";
    if (lex == "(") return "LPAREN";
    if (lex == ")") return "RPAREN";

    return "NOT FOUND"; 
}

Interpreter::Interpreter(std::unique_ptr<Syntax> node): root(std::move(node)) {};

void Interpreter::interpret() {
    ParserInterpreter parser_interpreter = ParserInterpreter(*root);
    
    parser_interpreter.generate();
}

void ParserInterpreter::generate_header() {
    header << "#ifndef PARSER_H_\n";
    header << "#define PARSER_H_\n";
    header << "#include <string>\n";
    header << "#include <vector>\n";
    header << "#include \"lexer.h\"\n";
    header << "#ifdef DEBUG\n";
    header << "\t#define TRACE() do { std::cout << \"ENTERING: \" << __func__ << \"\\n\"; } while(0)\n";
    header << "#else\n";
    header << "\t#define TRACE() do {} while (0)\n";
    header << "#endif\n";
    header << "\n";
    header << "class Parser {\n";
    header << "\tLexer lexer;\n";
    header << "\tvoid consume();\n";
    header << "\tvoid eat(TokenType expected_type);\n";
    header << "\tvoid match_terminal(std::string expected);\n";

    for (auto &rule : rules) {
        header << "\tvoid " << rule.first << "();\n";
    }

    header << "public:\n";
    header << "\tToken current_token;\n";
    header << "\tParser(Lexer given_lexer);\n";
    header << "void parse();\n";
    header << "};\n";
    header << "#endif // !PARSER_H_";
}

ParserInterpreter::ParserInterpreter(Syntax &node): root(node) {
    file.open("parser.cpp");
    header.open("parser.h");

    if (!file.is_open()) {
        std::cerr << "Error in creating parser file\n";
        abort();
    } else if (!header.is_open()) {
        std::cerr << "Error in creating parser header file\n";
        abort();
    }
};

void ParserInterpreter::visit(Syntax &node) {
    VISIT_TRACE();
    
    for (auto &rule : node.rules) {
        rule->accept(*this);
    }

}

void ParserInterpreter::visit(Rule &node) {
    VISIT_TRACE();
    
    file << "void Parser::" << node.name << "() {\n";
    file << "TRACE()\n";
    
    node.expr->accept(*this);

    file << "}\n";
}

void ParserInterpreter::visit(Expr &node) {
    VISIT_TRACE();
    
    int count = 0;
    if (node.alternatives.size() == 1) {
        node.alternatives[0]->accept(*this); 
    } else {
        for (auto &sequence : node.alternatives) {
            TerminalFinder finder = TerminalFinder(*sequence, rules);
            std::vector<std::string> terminals = finder.find_first_terminals();
            
            if (count == 0) {
                file << "if ";
            } else {
                file << "else if ";
            }
            
<<<<<<< HEAD
            for (std::string terminal : terminals) {
                if (terminal == terminals.front()) {
=======
            for (size_t i = 0; i < terminals.size(); i++) {
                std::string terminal = terminals[i];

                if (i == 0) {
>>>>>>> development
                    file << "(current_token.lexeme == \"" << terminal << "\"";
                } else {
                    file << " || current_token.lexeme == \"" << terminal << "\"";
                }
            }

            file << ") {\n";

            sequence->accept(*this);

            file << "}\n";
            count++;
        }
    }
    
}

void ParserInterpreter::visit(Sequence &node) {
    VISIT_TRACE();

    for (auto &term : node.terms) {
        term->accept(*this); 
    }
}

void ParserInterpreter::visit(Terminal &node) {
    VISIT_TRACE();
    std::string token_type = token_enum_for_terminal(node.lexeme);
    if (token_type == "NOT FOUND") {
        file << "match_terminal(\"" << node.lexeme << "\");\n";
    } else {
        file << "eat(" << token_enum_for_terminal(node.lexeme) << ");\n";
    }

}
void ParserInterpreter::visit(Nonterminal &node) {
    VISIT_TRACE();

    file << node.rule << "();\n";
}

void ParserInterpreter::visit(Optional &node) {
    VISIT_TRACE();
    
    int count = 0;
    for (auto &sequence : node.expr->alternatives) {
        TerminalFinder finder = TerminalFinder(*sequence, rules);
        std::vector<std::string> terminals = finder.find_first_terminals();

        if (count == 0) {
            file << "if (";
        } else {
            file << "else if (";
        }

        for (size_t i = 0; i < terminals.size(); i++) {
            if (i > 0) {
                file << " || ";
            } 
               
            file << "current_token.lexeme == \"" << terminals[i] << "\"";
        }

        file << ") {\n";
        sequence->accept(*this);
        file << "}\n";
        count++;
    }
}

void ParserInterpreter::visit(Repeated &node) {
    VISIT_TRACE();
    
    std::vector<std::string> all_terminals;

    for (auto &sequence : node.expr->alternatives) {
        TerminalFinder finder = TerminalFinder(*sequence, rules);
        std::vector<std::string> terminals = finder.find_first_terminals();
        
        for (std::string terminal : terminals) {
            if (std::find(all_terminals.begin(), all_terminals.end(), terminal) == all_terminals.end()) {
                all_terminals.push_back(terminal);
            }
        } 
    }
    
    file << "while (";
    for (size_t i = 0; i < all_terminals.size(); i++) {
        if (i > 0) {
            file << " || ";
        }
        file << "current_token.lexeme == \"" << all_terminals[i] << "\"";
    } 

    file << ") {\n";
    
    node.expr->accept(*this);

    file << "}\n";
}

void ParserInterpreter::visit(Grouped &node) {
    VISIT_TRACE();

    node.expr->accept(*this);
}

void ParserInterpreter::visit(Empty &node) {
    VISIT_TRACE();
}

void ParserInterpreter::create_rule_table() {
    for (auto &rule : root.rules) {
        rules[rule->name] = rule.get();
    }
}

void ParserInterpreter::generate() {
    //CONSUME
    file << "void Parser::consume() {\n";
    file << "\tcurrent_token = lexer.get_next_token();\n";
    file << "}\n";

    // EAT
    file << "void Parser::eat(TokenType expected_type) {\n";
    file << "\t#ifdef DEBUG\n";
    file << "\t\tstd::cout << \"Token: \" << current_token << \"\\n\";\n";
    file << "\t#endif // DEBUG\n";
    file << "\tif (expected_type != current_token.type) {\n";
    file << "\t\tstd::cout << \"Expected: \" << expected_type << \", Received: \" << current_token.type << \"\\n\";\n";
    file << "\t\tparse_errors++;\n";
    file << "\t}\n";
    file << "\tconsume();\n";
    file << "}\n";

    //MATCH TERMINAL
<<<<<<< HEAD
    file << "void match_terminal(std::string expected) {\n";
    file << "\tif (current_token.type != Token::TERMINAL || current_token.lexeme != expected) {\n";
    file << "\t\tstd::cout << \"Expected: \" << expected << \", Received \" << current_token.lexeme << \"\\n\";\n";
    file << "\t\tparse_errors++;\n";
=======
    file << "void Parser::match_terminal(std::string expected) {\n";
    file << "\tif (current_token.lexeme != expected) {\n";
    file << "\t\tstd::cerr << \"Expected: \" << expected << \", Received \" << current_token.lexeme << \"\\n\";\n";
    file << "\t\tabort();\n";
>>>>>>> development
    file << "\t}\n";
    file << "\tconsume();\n";
    file << "}\n";

    create_rule_table();
    generate_header();

<<<<<<< HEAD
    root->accept(*this);

    file << "void parse() {\n";
    file << "\t" << root->rules[0]->name << "();\n";
    file << "\tstd::cout << \"There were \" << parse_errors << \" parse errors\\n\"\n";
    file << "}";
=======
    root.accept(*this);
>>>>>>> development
}

TerminalFinder::TerminalFinder(Sequence &node, std::unordered_map<std::string, Rule*> &given_rules): root(node), rules(given_rules) {};


void TerminalFinder::visit(Syntax &node) {
    std::cerr << "Given a syntax node, never should happend\n";
    abort();
}

void TerminalFinder::visit(Rule &node) {
    node.expr->accept(*this);
}

void TerminalFinder::visit(Expr &node) {
    for (auto &alternative : node.alternatives) {
        can_append = true;
        alternative->accept(*this); 
    }
}

void TerminalFinder::visit(Sequence &node) {
    can_append = true;
    if (!node.terms.empty()) {
        node.terms[0]->accept(*this);
    }
}

void TerminalFinder::visit(Terminal &node) {
    if (can_append) {
        terminals.push_back(node.lexeme);
        can_append = false;
    }
}

void TerminalFinder::visit(Nonterminal &node) {
    std::string rule = node.rule;
    auto iter = rules.find(node.rule);
    if (iter == rules.end()) {
        std::cerr << "No definition for rule '" << rule << "'\n";
        abort();
    }

    Rule* target = iter->second;
    target->accept(*this);
}

void TerminalFinder::visit(Optional &node) {
    node.expr->accept(*this);
}

void TerminalFinder::visit(Repeated &node) {
    node.expr->accept(*this);
}

void TerminalFinder::visit(Grouped &node) {
    node.expr->accept(*this);
}

void TerminalFinder::visit(Empty &node) {
    return;
}

std::vector<std::string> TerminalFinder::find_first_terminals() {
    terminals.clear();
    can_append = true;
    root.accept(*this);

    return terminals;
}

