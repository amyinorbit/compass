#include <iostream>
#include <string>
#include <fstream>
#include <apfun/maybe.hpp>
#include <compass/compiler/assembler.hpp>

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

void compile(const string& source) {
    Assembler casm(source);
    casm.compile();
}


int main(int argc, const char** argv) {

    if(argc != 2) {
        std::cerr << "usage: casm <input-file>\n";
        return -1;
    }

    read(argv[1]).map(compile).or_else([=]{
        std::cerr << "could not open " << argv[2] << " for reading.\n";
    });

    return 0;
}
