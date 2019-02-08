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

Result<std::string> getPath(int argc, const char** args) {
    if(argc != 2) {
        return Error("usage: " + std::string(args[0]) + " story_file.txt");
    }
    return std::string(args[1]);
}

Result<std::string> readSource(const std::string& path) {
    std::ifstream in(path);
    if(!in.is_open()) {
        return Error("unable to open file '" + path + "'");
    }
    return std::string(std::istreambuf_iterator<char>(in), {});
}

void runGame(const std::string& source) {
    Compass::BasicEnglish grammar;
    Compass::StoryParser parser(source, grammar);
    auto story = parser.compile();

    StreamIO io;
    Game game(story, io);
    game.start();
    for(;;) game.update();
}

int main(int argc, const char** args) {
    getPath(argc, args)
        .flatMap(readSource)
        .map(runGame)
        .mapError([](const auto& error){
            std::cerr << "error: " << error.description << "\n";
        });
}