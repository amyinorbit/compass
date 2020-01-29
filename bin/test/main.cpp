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
    InferEngine infer(compiler);

    for(;;) {
        std::cout << "input> ";
        if(!std::getline(std::cin, in)) break;

        Parser ap(string(in.data(), in.size()), compiler, infer);

        ap.parse();

        compiler.diagnose(std::cout);

        if(!compiler.isFailed()) {
            infer.dump();
            {
                auto out = std::ofstream("/home/amy/Desktop/test.bin", std::ostream::binary);
                infer.write(out);
                std::cout << "written\n";
            }

            {
                auto in = std::ifstream("/home/amy/Desktop/test.bin", std::ostream::binary);
                rt::Collector gc;
                // try {
                    Loader(gc, in).load();
                    std::cout << "read\n";
                // } catch(std::exception& e) {
                //     std::cerr << "Error: " << e.what() << "\n";
                // }
            }

        }
        compiler.flush();
    }

    return 0;
}
