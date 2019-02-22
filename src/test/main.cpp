#include <compass/story.hpp>
#include <compass/grammar.hpp>
#include <compass/game.hpp>
#include <compass/iomanaging.hpp>
#include <string>
#include <fstream>
#include <iostream>

#include <compass/compiler.hpp>

using namespace Compass;

StreamIO io;

result<std::string> getPath(StreamIO& io) {
    io.println("Compass Engine " COMPASS_VERSION_STRING " by Amy Parent");
    io.print("story file name> ");
    auto path = io.readLine();
    io.println("Trying to load '" + path + "'");
    io.println();
    return path;
}

result<std::string> readSource(const std::string& path) {
    std::ifstream in(path);
    if(!in.is_open()) {
        return make_unexpected("unable to open file '" + path + "'");
    }
    return std::string(std::istreambuf_iterator<char>(in), {});
}

result<Story> compile(const std::string& path) {
    Compiler compiler;
    return Compiler().compile(path);
}

void runGame(Story story) {
    Game game(story, io);
    game.start();
    for(;;) game.update();
}

int main(int argc, const char** args) {
    getPath(io)
        .and_then(compile)
        .map(runGame)
        .map_error([](const auto& error) -> void {
            std::cerr << "error: " << error << "\n";
        });
}