#include <iostream>
#include <string>
#include <compass/language/lexer.hpp>
#include <compass/compiler/compiler.hpp>
#include <compass/compiler/parser.hpp>
#include <compass/runtime2/memory.hpp>

using namespace amyinorbit;
using namespace amyinorbit::compass;

int main(int argc, const char** argv) {

    Memory pool(1024);
    auto obj = pool.alloc(10);
    auto obj2 = pool.alloc(10);
    pool.alloc(20);

    pool.debug("alloc 1");
    pool.dealloc(obj2);
    pool.debug("dealloc obj2");

    pool.dealloc(obj);
    pool.debug("dealloc obj");
    pool.alloc(4);

    pool.debug("alloc 2");
    pool.alloc(20);

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
