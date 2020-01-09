#include <iostream>
#include <string>
#include <compass/language/lexer.hpp>

using namespace amyinorbit::compass;

int main(int argc, const char** argv) {

    std::string in;

    for(;;) {
        std::cout << "input> ";
        if(!std::getline(std::cin, in)) break;
        Lexer lex(amyinorbit::string(in.data(), in.size()));
        auto tok = lex.nextToken();

        std::cout << "tokens:\n";
        while(tok.kind != Token::Kind::End) {
            std::cout << "   -" << tok.type() << ": " << tok.text << "\n";
            tok = lex.nextToken();
        }

        std::cout << "\n";
    }

    return 0;
}
