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
        diagnostic(Diagnostic(Diagnostic::Error, message));
    }
    
    void Compiler::diagnostic(const Diagnostic& diag) {
        consumer_(diag);
    }
    
    result<Story> Compiler::compile(const std::string& path) {
        consumer_(Diagnostic(Diagnostic::Progress, "compiling '" + path + "'"));
        return getFileContents(path).and_then([this](const auto& source) {
            auto invocation = Parser(source, *this);
            invocation.run();
            return sema_.resolve();
        });
    }
    
    bool Compiler::use(const std::string& libname) {
        return include(libdir_/makePath(libname));
    }
    
    bool Compiler::include(const Filesystem::Path& path) {
        const auto fullPath = libdir_/path;
        consumer_(Diagnostic(Diagnostic::Progress, "compiling '" + fullPath.get() + "'"));
        return getFileContents(fullPath).map([this](const auto& source) {
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
        std::ifstream in(path.get());
        if(!in.is_open()) {
            return make_unexpected("unable to open file '" + path.get() + "'");
        }
        return std::string(std::istreambuf_iterator<char>(in), {});
    }
}
