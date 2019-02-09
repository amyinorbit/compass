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

StreamIO io;

Result<std::string> getPath(StreamIO& io) {
    io.println("Compass Engine 0.1 by Amy Parent");
    io.print("story file name> ");
    auto path = io.readLine();
    io.println("Trying to load '" + path + "'");
    io.println();
    return path;
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
    
    Game game(story, io);
    game.start();
    for(;;) game.update();
}

int main(int argc, const char** args) {
    getPath(io)
        .flatMap(readSource)
        .map(runGame)
        .mapError([](const auto& error){
            std::cerr << "error: " << error.description << "\n";
        });
}