#include <iostream>
#include <string>
#include <compass/language/lexer.hpp>
#include <compass/compiler/compiler.hpp>
#include <compass/compiler/parser.hpp>

using namespace amyinorbit;
using namespace amyinorbit::compass;

int main(int argc, const char** argv) {

    std::string in;

    Compiler compiler;
    InferEngine infer(compiler);

    for(;;) {
        std::cout << "input> ";
        if(!std::getline(std::cin, in)) break;

        Parser ap(string(in.data(), in.size()), compiler, infer);

        ap.parse();

        compiler.diagnose(std::cout);

        if(!compiler.isFailed()) {
            infer.dump();
        }
        compiler.flush();
    }

    return 0;
}
