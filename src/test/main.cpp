#include <compass/storyparser.hpp>
#include <compass/story.hpp>
#include <compass/lexer.hpp>
#include <compass/grammar.hpp>
#include <compass/sentence.hpp>
#include <algorithm>
#include <set>
#include <string>
#include <fstream>
#include <iostream>

void display(Compass::Story& story, const Compass::Context& ctx, const Compass::Place& place) {
    std::cout << "\n* " << story.string(place.name) << "\n";
    std::cout << story.string(place.description) << "\n\n";
    for(auto id: place.things) {
        const auto& thing = ctx.thing(id);
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
    
    Compass::Context ctx = story.prototype;
    
    Compass::Place* current = &ctx.start();
    display(story, ctx, *current);
    
    for(;;) {
        std::cout << "> ";
        std::string phrase;
        std::getline(std::cin, phrase);
        
        Compass::Sentence sentence(story, phrase, grammar);
        bool success;
        Compass::Sentence::Command cmd;
        std::tie(success, cmd) = sentence.parse();
        
        if(!success) {
            std::cout << "I can't understand that\n";
            continue;
        }
        
        if(cmd.verb != "go" && cmd.verb != "Go") {
            std::cout << "I don't quite understand that verb\n";
            continue;
        }
        
        const auto it = std::find_if(current->links.begin(), current->links.end(), [&](const Compass::Link& link) {
            return cmd.object == link.direction;
        });
        if(it == current->links.end()) {
            std::cout << "you can't go that way\n";
        }
        else {
            current = &ctx.place(it->target);
            
            display(story, ctx, *current);
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
    // return testCompile("story.txt");
}