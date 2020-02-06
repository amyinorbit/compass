#include <iostream>
#include <string>
#include <fstream>
#include <compass/language/lexer.hpp>
#include <compass/compiler/compiler.hpp>
#include <compass/compiler/parser.hpp>
#include <compass/runtime2/collector.hpp>
#include <compass/runtime2/unpack.hpp>

using namespace amyinorbit;
using namespace amyinorbit::compass;

int main(int argc, const char** argv) {

    std::string in;
    Compiler compiler;
    sema::Sema sema(compiler);
    InferEngine infer(compiler, sema);

    for(;;) {
        std::cout << "input> ";
        if(!std::getline(std::cin, in)) break;

        Parser ap(string(in.data(), in.size()), compiler, infer);

        ap.parse();

        compiler.diagnose(std::cout);

        if(!compiler.is_failed()) {
            infer.dump();
        }
        compiler.flush();
    }

    return 0;
}
