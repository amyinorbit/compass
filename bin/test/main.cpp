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

    string a, b, c, d;
    a = "hello";
    b = "world";
    c = "hello";
    d = "hell";

    std::cout << a << "==" << b << "=" << (a==b) << "\n";
    std::cout << a << "==" << c << "=" << (a==c) << "\n";
    std::cout << d << "==" << a << "=" << (d==a) << "\n";

    for(;;) {
        std::cout << "input> ";
        if(!std::getline(std::cin, in)) break;

        Parser ap(string(in.data(), in.size()), compiler, infer);

        ap.parse();

        compiler.diagnose(std::cout);
        compiler.flush();
    }

    return 0;
}
