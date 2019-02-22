//===--------------------------------------------------------------------------------------------===
// driver.cpp - Implementation of the compiler driver
// This source is part of the Compass Engine
//
// Created on 2019-02-22 by Amy Parent <amy@amyparent.com>
// Copyright (c) 2019 Amy Parent
// Licensed under the MIT License
// =^•.•^=
//===--------------------------------------------------------------------------------------------===
#include <fstream>
#include <compass/compiler.hpp>
#include <compass/parser.hpp>
#include <boost/range/adaptors.hpp>
#include <boost/range/algorithm.hpp>

namespace Compass {
    
    
    void Compiler::error(const std::string& message) {
        error_.emplace(message);
    }
    
    result<Story> Compiler::compile(const std::string& path) {
        return getFileContents(path).and_then([this](const auto& source) {
            auto invocation = Parser(source, *this);
            invocation.run();
            return sema_.resolve();
        });
    }
    
    bool Compiler::include(const std::string& path) {
        return getFileContents(makeFileName(path)).map([this](const auto& source) {
            auto invocation = Parser(source, *this);
            invocation.run();
        }).has_value();
    }
    
    std::string Compiler::makeFileName(const std::string& name) {
        using namespace boost::adaptors;
        // TODO:    dear god please change that. We should at least tack on the root folder of the
        //          "invoked" file.
        std::string path;
        boost::copy(
            name | transformed([](char c) { return c == ' ' ? '-' : c; }),
            std::back_inserter(path)
        );
        return path + ".txt";
    }
    
    result<std::string> Compiler::getFileContents(const std::string& path) {
        std::cout << "compiling: " << path << "\n";
        std::ifstream in(path);
        if(!in.is_open()) {
            return make_unexpected("unable to open file '" + path + "'");
        }
        return std::string(std::istreambuf_iterator<char>(in), {});
    }
}
