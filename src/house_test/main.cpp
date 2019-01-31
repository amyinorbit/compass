#include <house/parser.hpp>
#include <fstream>
#include <iostream>

int main() {
    std::ifstream in("test.txt");
    if(in.is_open()) {
        House::AssetParser parser("room_test.txt", in);
        parser.compile();
    } else {
        std::cerr << "unable to open file\n";
    }
}