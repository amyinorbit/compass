#include <iostream>
#include <string>
#include <fstream>
#include <apfun/maybe.hpp>
#include <compass/compiler/compiler.hpp>
#include <compass/compiler/parser.hpp>

using namespace amyinorbit;
using namespace amyinorbit::compass;

maybe<string> read(const std::string& path) {
    std::ifstream in(path);
    if(!in.is_open()) return nothing();
    string data;
    data.reserve(1024);

    char c;
    while(in.get(c)) {
        data += c;
    }
    return data;
}

maybe<sema::Sema> compile(const string& source) {
    Compiler comp;
    Compiler compiler;
    sema::Sema sema(compiler);
    InferEngine infer(compiler, sema);

    Parser(source, compiler, infer).parse();

    compiler.diagnose(std::cout);
    if(compiler.is_failed()) return nothing();

    sema.print_index();
    return std::move(sema);
}

auto write(std::ostream& out) {
    return [&](const sema::Sema& sema) {
        sema.write(out);
    };
}


int main(int argc, const char** argv) {

    if(argc != 2) {
        std::cerr << "usage: casm <input-file>\n";
        return -1;
    }

    std::string in_path(argv[1]);
    std::string out_path = in_path + ".story";
    std::ofstream out_file(out_path);

    read(argv[1])
        .flat_map(compile)
        .map(write(out_file))
        .or_else([=]{ std::cerr << "could not open " << argv[2] << " for reading.\n"; });

    return 0;
}
