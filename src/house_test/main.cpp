#include <house/assetparser.hpp>
#include <house/lexer.hpp>
#include <house/grammar.hpp>
#include <fstream>
#include <iostream>

int main() {
    std::ifstream in("test.txt");
    if(in.is_open()) {
        std::string source(std::istreambuf_iterator<char>(in), {});
        
        House::BasicEnglish grammar;
        House::AssetParser parser(source, grammar);
        parser.compile();
        
    } else {
        std::cerr << "unable to open file\n";
    }
}