//===--------------------------------------------------------------------------------------------===
// sentence.hpp - Sentence parser API for the Compass engine
// This source is part of the Compass Engine
//
// Created on 2019-02-05 by Amy Parent <amy@amyparent.com>
// Copyright (c) 2019 Amy Parent
// Licensed under the MIT License
// =^•.•^=
//===--------------------------------------------------------------------------------------------===
#pragma once
#include <string>
#include <utility>
#include <compass/language/driver.hpp>
#include <compass/language/grammar.hpp>
#include <compass/language/lexer.hpp>
#include <compass/language/rdparser.hpp>
#include <compass/core/functional.hpp>
#include <compass/core/story.hpp>

namespace Compass {
    class Sentence: public RDParser, public Driver {
    public:
        
        struct Command {
            std::string verb;
            std::string preposition;
            std::string object;
        };
        
        Sentence(const Story& story, const std::string& data, const Grammar& grammar);
        virtual ~Sentence() {}
        
        result<Command> parse();
        
    private:
        virtual void error(const std::string& message);
        
        virtual const Grammar&  grammar() const { return grammar_; }
        virtual bool isFailed() const { return error_.has_value(); }
        
        const Grammar& grammar_;
        const Story& story_;
        optional<std::string> error_;
    };
}
