//#include <house/parser.hpp>
#include <house/lexer.hpp>
#include <fstream>
#include <iostream>

int main() {
    std::ifstream in("test.txt");
    if(in.is_open()) {
        std::string source(std::istreambuf_iterator<char>(in), {});
        House::Lexer lexer(source);
        
        do {
            const auto& tok = lexer.nextToken();
            std::cout << "[" << tok.type() << "] " << tok.text << "\n";
        } while(lexer.currentToken().kind != House::Token::End);
        
    } else {
        std::cerr << "unable to open file\n";
    }
}