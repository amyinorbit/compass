//===--------------------------------------------------------------------------------------------===
// driver.hpp - Compass's compiler driver, useful for non-trivial compilation (includes)
// This source is part of the Compass Engine
//
// Created on 2019-02-22 by Amy Parent <amy@amyparent.com>
// Copyright (c) 2019 Amy Parent
// Licensed under the MIT License
// =^•.•^=
//===--------------------------------------------------------------------------------------------===
#pragma once
#include <string>
#include <compass/compiler/sema.hpp>
#include <compass/core/functional.hpp>
#include <compass/core/story.hpp>
#include <compass/language/driver.hpp>
#include <compass/language/grammar.hpp>
#include <compass/filesystem/path.hpp>

namespace Compass::Compiler {
    
    class Compiler: public Language::Driver {
    public:
    
        // TODO: provide a way to give a custom filename resolver here
        Compiler(const Filesystem::Path& libdir): libdir_(libdir.canonical()) {}
        virtual ~Compiler() {}
    
        result<Story> compile(const std::string& path);
        bool use(const std::string& libname);
        bool include(const Filesystem::Path& path);
    
        Sema& sema() { return sema_; }
        const Sema& sema() const { return sema_; }
        virtual const Language::Grammar& grammar() const { return grammar_; }
    
        virtual void error(const std::string& message);
        virtual bool isFailed() const { return error_.has_value(); }
    
    private:
        using Error = std::string;
    
        Filesystem::Path makePath(const std::string& name);
        result<std::string> getFileContents(const Filesystem::Path& path);
    
        Filesystem::Path        libdir_;
        Language::BasicEnglish  grammar_;
        Sema                    sema_;
        optional<Error>         error_ = {};
    };
}
