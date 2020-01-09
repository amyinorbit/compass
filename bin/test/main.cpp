#include <iostream>
#include <string>
#include <compass/language/lexer.hpp>
#include <compass/compiler/compiler.hpp>
#include <compass/compiler/parser.hpp>

using namespace amyinorbit;
using namespace amyinorbit::compass;

int main(int argc, const char** argv) {

    std::string in;

    for(;;) {
        std::cout << "input> ";
        if(!std::getline(std::cin, in)) break;

        Compiler compiler;
        AssertionParser ap(string(in.data(), in.size()), compiler);

        ap.sentence();

        compiler.diagnose(std::cout);
    }

    return 0;
}
