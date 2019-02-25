//===--------------------------------------------------------------------------------------------===
// main.cpp - Simple Compiler demo for the Compass engine
// This source is part of the Compass Engine
//
// Created on 2019-02-25 by Amy Parent <amy@amyparent.com>
// Copyright (c) 2019 Amy Parent
// Licensed under the MIT License
// =^•.•^=
//===--------------------------------------------------------------------------------------------===
#include <string>
#include <iostream>
#include <fstream>
#include <compass/compiler/compiler.hpp>
#include <compass/core/file.hpp>
#include <compass/core/functional.hpp>
#include <compass/core/iomanaging.hpp>
#include <compass/core/story.hpp>
#include <compass/filesystem/path.hpp>
#include "json.hpp"

using namespace Compass;
using json = nlohmann::json;
StreamIO io;

struct Params {
    enum Kind { Out, Lib };
    
    Filesystem::Path in = {};
    Filesystem::Path out = {};
    Filesystem::Path lib = {};
};

struct Output {
    Story story;
    Filesystem::Path path;
};

bool assign(const json& manifest, Filesystem::Path& v, const std::string& key) {
    if(manifest.find(key) == manifest.end()) return false;
    v = Filesystem::Path(manifest[key].get<std::string>());
    return true;
}

using Language::Diagnostic;

void printDiagnostics(const Diagnostic& diag) {
    switch(diag.level()) {
    case Diagnostic::Progress:
        std::cout << "[" << diag.title() << "]" << std::endl;
        break;
        
    case Diagnostic::Info:
        std::cout << " * " << diag.title() << std::endl;
        break;
        
    case Diagnostic::Error:
        std::cout << "***" << std::endl;
        std::cout << "[error: " << diag.title() << "]" << std::endl;
        if(diag.message()) {
            std::cout << "***" << std::endl;
            std::cout << *diag.message() << std::endl;
        }
        break;
    }
}

result<Params> getParams(int argc, const char** argv) {
    if(argc != 2) return make_unexpected("invalid parameters");
    
    Filesystem::Path manifestPath{std::string(argv[1])};
    auto projectPath = manifestPath.directory();
    
    Params params;
    
    std::ifstream data(manifestPath.get());
    if(!data.is_open()) return make_unexpected("cannot open project file " + manifestPath.get());
    
    json manifest;
    data >> manifest;
    
    if(!assign(manifest, params.in, "story_source"))
        return make_unexpected("missing 'story_source' in project file");
    
    if(!assign(manifest, params.lib, "libraries_dir"))
        return make_unexpected("missing 'story_source' in project file");
    
    if(!assign(manifest, params.out, "output"))
        return make_unexpected("missing 'story_source' in project file");
    
    
    return params;
}

result<Output> compile(Params params) {
    Compiler::Compiler compiler(params.lib, &printDiagnostics);
    compiler.include(Filesystem::Path("standard.txt"));
    return compiler.compile(params.in.get()).map([&](const auto& story) {
        return Output{story, params.out};
    });
}

result<void> writeResults(Output out) {
    std::ofstream file(out.path.get(), std::ios::binary);
    if(!file.is_open()) return make_unexpected("cannot open " + out.path.get() + " for writing");
    writeStory(file, out.story);
    return {};
}


int main(int argc, const char** argv) {
    io.println("Compass Compiler " COMPASS_VERSION_STRING " by Amy Alex Parent");
    getParams(argc, argv)
        .and_then(compile)
        .and_then(writeResults)
        .map_error([](const auto& error) -> void {
            io.println("error: " + error + "\n");
        });
    io.println("[done]");
}
