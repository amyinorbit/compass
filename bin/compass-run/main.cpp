#include <compass/core/story.hpp>
#include <compass/core/file.hpp>
#include <compass/runtime/game.hpp>
#include <string>
#include <fstream>
#include <iostream>

using namespace Compass;
StreamIO io;

result<std::string> getPath(StreamIO& io) {
    io.print("story file name> ");
    auto path = io.readLine();
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

result<Story> loadFile(const std::string& path) {
    std::ifstream in(path, std::ios::binary);
    if(!in.is_open()) return make_unexpected("cannot open story file " + path);
    return loadStory(in);
}

void runGame(const Story& story) {
    Runtime::Game game(story, io);
    game.start();
    for(;;) game.update();
}

int main(int argc, const char** args) {
    io.println("Compass Runtime " COMPASS_VERSION_STRING " by Amy Alex Parent Parent");
    getPath(io)
        .and_then(loadFile)
        .map(runGame)
        .map_error([](const auto& error) -> void {
            std::cerr << "error: " << error << "\n";
        });
}