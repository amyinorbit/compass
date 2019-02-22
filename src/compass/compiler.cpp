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

namespace Compass {
    
    
    void Compiler::error(const std::string& message) {
        error_.emplace(message);
    }
    
    result<Story> Compiler::compile(const std::string& path) {
        return getFileContents(path).and_then([this](const auto& source) {
            auto invocation = Parser(source, *this, sema_);
            invocation.run();
            return sema_.resolve();
        });
    }
    
    bool Compiler::include(const std::string& path) {
        // TODO: implementation!
        return false;
    }
    
    result<std::string> Compiler::getFileContents(const std::string& path) {
        std::ifstream in(path);
        if(!in.is_open()) {
            return make_unexpected("unable to open file '" + path + "'");
        }
        return std::string(std::istreambuf_iterator<char>(in), {});
    }
}
