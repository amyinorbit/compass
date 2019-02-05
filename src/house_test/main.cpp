#include <house/storyparser.hpp>
#include <house/story.hpp>
#include <house/lexer.hpp>
#include <house/grammar.hpp>
#include <algorithm>
#include <set>
#include <string>
#include <fstream>
#include <iostream>

void display(House::Story& story, const House::Place& place) {
    std::cout << "\n* " << story.string(place.name) << "\n";
    std::cout << story.string(place.description) << "\n\n";
    for(auto id: place.things) {
        const auto& thing = story.thing(id);
        std::cout << "There is " << story.string(thing.article) << " " << story.string(thing.name) << "\n";
    }
}

int testCompile(const std::string& path) {
    std::ifstream in(path);
    if(!in.is_open()) {
        std::cerr << "unable to open file\n";
        return -1;
    }
    std::string source(std::istreambuf_iterator<char>(in), {});
    
    House::BasicEnglish grammar;
    House::StoryParser parser(source, grammar);
    auto story = parser.compile();
    
    House::Place* current = &story.place(story.start);
    
    display(story, *current);
    
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
            
            display(story, *current);
        } 
    }
    
    return 0;
}

int main(int argc, const char** args) {
    if(argc != 2) {
        std::cerr << "usage: " << args[0] << " story_file.txt\n";
        return -1;
    }
    return testCompile(args[1]);
}