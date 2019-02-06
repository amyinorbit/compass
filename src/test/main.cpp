#include <compass/storyparser.hpp>
#include <compass/story.hpp>
#include <compass/lexer.hpp>
#include <compass/grammar.hpp>
#include <compass/sentence.hpp>
#include <compass/run.hpp>
#include <algorithm>
#include <set>
#include <string>
#include <fstream>
#include <iostream>

void display(const Compass::Run& run) {
    const auto& place = run.current();
    std::cout << "\n* " << run.string(place.name) << "\n";
    std::cout << run.string(place.description) << "\n\n";
    for(auto id: place.things) {
        const auto& thing = run.thing(id);
        std::cout << "There is " << run.string(thing.article) << " " << run.string(thing.name) << "\n";
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
    
    Compass::Run run(story);
    display(run);
    
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
        const auto& current = run.current();
        const auto it = std::find_if(current.links.begin(), current.links.end(), [&](const Compass::Link& link) {
            return cmd.object == link.direction;
        });
        if(it == current.links.end()) {
            std::cout << "you can't go that way\n";
        }
        else {
            run.go(it->target);
            display(run);
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