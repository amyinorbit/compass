#include <house/storyparser.hpp>
#include <house/story.hpp>
#include <house/lexer.hpp>
#include <house/grammar.hpp>
#include <algorithm>
#include <set>
#include <string>
#include <fstream>
#include <iostream>

int testCompile() {
    std::ifstream in("test.txt");
    if(!in.is_open()) {
        std::cerr << "unable to open file\n";
        return -1;
    }
    std::string source(std::istreambuf_iterator<char>(in), {});
    
    House::BasicEnglish grammar;
    House::StoryParser parser(source, grammar);
    auto story = parser.compile();
    
    House::Place* current = &story.place(story.start);

    std::cout << "\n* " << story.string(current->name) << "\n";
    std::cout << story.string(current->description) << "\n\n";
    // std::cout << "(" << current->links.size() << " links)\n";
    
    for(;;) {
        std::cout << "> ";
        std::string cmd;
        std::getline(std::cin, cmd);
        //std::cin >> cmd;
        
        
        
        const auto it = std::find_if(current->links.begin(), current->links.end(), [&](const House::Link& link) {
            return cmd == link.direction;
        });
        if(it == current->links.end()) {
            std::cout << "you can't go that way\n";
        }
        else {
            std::cout << "going to " << story.string(it->target) << "\n";
            current = &story.place(it->target);
            
            std::cout << "\n* " << story.string(current->name) << "\n";
            std::cout << story.string(current->description) << "\n\n";
            // std::cout << "(" << current->links.size() << " links\n";
        } 
    }
    
    return 0;
}

int main() {
    return testCompile();
}