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
#include <compass/compiler/compiler.hpp>
#include <compass/compiler/parser.hpp>
#include <boost/range/adaptors.hpp>
#include <boost/range/algorithm.hpp>

namespace Compass::Compiler {
    
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
    
    bool Compiler::use(const std::string& libname) {
        return getFileContents(libdir_/makePath(libname)).map([this](const auto& source) {
            auto invocation = Parser(source, *this);
            invocation.run();
        }).has_value();
    }
    
    bool Compiler::include(const Filesystem::Path& path) {
        return getFileContents(libdir_/path).map([this](const auto& source) {
            auto invocation = Parser(source, *this);
            invocation.run();
        }).has_value();
    }
    
    Filesystem::Path Compiler::makePath(const std::string& name) {
        using namespace boost::adaptors;
        
        std::string path;
        boost::copy(
            name | transformed([](char c) { return c == ' ' ? '-' : c; }),
            std::back_inserter(path)
        );
        return Filesystem::Path(path + ".txt");
    }
    
    result<std::string> Compiler::getFileContents(const Filesystem::Path& path) {
        std::cout << "compiling: " << path.get() << "\n";
        std::ifstream in(path.get());
        if(!in.is_open()) {
            return make_unexpected("unable to open file '" + path.get() + "'");
        }
        return std::string(std::istreambuf_iterator<char>(in), {});
    }
}
