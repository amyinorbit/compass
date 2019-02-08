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

int compassAppMain(const std::string& path) {
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
    
    for(;;) game.update();
    
    return 0;
}

int main(int argc, const char** args) {
    if(argc != 2) {
        std::cerr << "usage: " << args[0] << " story_file.txt\n";
        return -1;
    }
    return compassAppMain(args[1]);
}