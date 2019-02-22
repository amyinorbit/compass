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
#include <compass/driver.hpp>
#include <compass/grammar.hpp>
#include <compass/sema.hpp>
#include <compass/story.hpp>
#include <compass/utils/functional.hpp>

namespace Compass {
    
    class Compiler: public Driver {
    public:
        
        // TODO: provide a way to give a custom filename resolver here
        Compiler() {}
        virtual ~Compiler() {}
        
        result<Story> compile(const std::string& path);
        bool include(const std::string& path);
        
        Sema& sema() { return sema_; }
        virtual const Grammar& grammar() const { return grammar_; }
        
        virtual void error(const std::string& message);
        virtual bool isFailed() const { return error_.has_value(); }
        
    private:
        using Error = std::string;
        // TODO: provide an override point here. Probably need some path/FS library, Boost?
        std::string makeFileName(const std::string& name);
        result<std::string> getFileContents(const std::string& path);
        
        BasicEnglish    grammar_;
        Sema            sema_;
        optional<Error> error_ = {};
    };
}
