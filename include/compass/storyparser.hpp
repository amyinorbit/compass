//===--------------------------------------------------------------------------------------------===
// parser.hpp - Compass adventure asset parser
// This source is part of the Compass Engine
//
// Created on 2019-01-30 by Amy Parent <amy@amyparent.com>
// Copyright (c) 2019 Amy Parent
// Licensed under the MIT License
// =^•.•^=
//===--------------------------------------------------------------------------------------------===
#pragma once
#include <string>
#include <set>
#include <compass/grammar.hpp>
#include <compass/lexer.hpp>
#include <compass/rdparser.hpp>
#include <compass/semantics.hpp>
#include <compass/story.hpp>

namespace Compass {
    class StoryParser: RDParser {
    public:
        StoryParser(const std::string& data, const Grammar& grammar);
        Story compile();
    
    private:
        using Name = std::pair<std::string, std::string>;

        virtual void error(const std::string& message);
        
        void recDirectionDecl(Story& story);
        
        void recDecl(Story& story);
        Name recDeclStart(Story& story);
        
        void recRoomDecl(Story& story, Place& place);
        void recThingDecl(Story& story, Thing& thing);
        
        void recDirection(Story& story, Place& place);
        
        Action recAction(Story& story);
        
        std::string recWords(const std::string& stop = "");
        std::string recWords(Grammar::Class stop);
        
        
        Semantics sem_;
        // Lexer lex_;
        // const Grammar& grammar_;
    };
}

