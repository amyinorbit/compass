#include <compass/storyparser.hpp>
#include <compass/story.hpp>
#include <compass/lexer.hpp>
#include <compass/grammar.hpp>
#include <compass/sentence.hpp>
#include <compass/game.hpp>
#include <compass/run.hpp>
#include <compass/iomanaging.hpp>
#include <algorithm>
#include <set>
#include <string>
#include <fstream>
#include <iostream>

using namespace Compass;

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
    
    StreamIO io;
    Game game(story, io);
    game.start();
    // Compass::Run run(story);
    // display(run);
    
    for(;;) {
        game.update();
        // std::cout << "> ";
//         std::string phrase;
//         std::getline(std::cin, phrase);
//
//         Compass::Sentence sentence(story, phrase, grammar);
//         bool success;
//         Compass::Sentence::Command cmd;
//         //std::tie(success, cmd) = sentence.parse();
//
//         sentence.parse().flatMap([&](auto cmd) -> Result<Sentence::Command> {
//
//             if(cmd.verb != "go" && cmd.verb != "Go")
//                 return Error("I don't understand that verb");
//             return cmd;
//
//         }).flatMap([&](auto cmd) -> Result<Sentence::Command> {
//
//             const auto& current = run.current();
//             const auto it = std::find_if(current.links.begin(), current.links.end(), [&](const Compass::Link& link) {
//                 return cmd.object == link.direction;
//             });
//             if(it == current.links.end()) return Error("You can't go that way");
//             run.go(it->target);
//             display(run);
//             return cmd;
//
//         }).mapError([=](auto error){
//
//             std::cout << error.description << "\n";
//
//         });
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