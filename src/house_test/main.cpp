#include <compass/storyparser.hpp>
#include <compass/story.hpp>
#include <compass/lexer.hpp>
#include <compass/grammar.hpp>
#include <algorithm>
#include <set>
#include <string>
#include <fstream>
#include <iostream>

void display(Compass::Story& story, const Compass::Place& place) {
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
    
    Compass::BasicEnglish grammar;
    Compass::StoryParser parser(source, grammar);
    auto story = parser.compile();
    
    Compass::Place* current = &story.place(story.start);
    
    display(story, *current);
    
    for(;;) {
        std::cout << "> ";
        std::string cmd;
        std::getline(std::cin, cmd);
        //std::cin >> cmd;
        
        const auto it = std::find_if(current->links.begin(), current->links.end(), [&](const Compass::Link& link) {
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