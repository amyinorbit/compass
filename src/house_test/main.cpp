#include <house/storyscript.hpp>
#include <house/lexer.hpp>
#include <house/grammar.hpp>
#include <set>
#include <string>
#include <fstream>
#include <iostream>

void testCompile() {
    std::ifstream in("test.txt");
    if(in.is_open()) {
        std::string source(std::istreambuf_iterator<char>(in), {});
        
        House::BasicEnglish grammar;
        House::StoryParser parser(source, grammar);
        parser.compile();
    } else {
        std::cerr << "unable to open file\n";
    }
}

int main() {
    testCompile();
}