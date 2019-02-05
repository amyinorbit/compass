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
#include <map>
#include <house/grammar.hpp>
#include <house/lexer.hpp>
#include <house/semantics.hpp>
#include <house/story.hpp>

namespace Compass {
    class StoryParser {
    public:
        StoryParser(const std::string& data, const Grammar& grammar);
        Story compile();
    
    private:
    
        // struct Direction {
        //     std::string direction;
        //     std::string opposite;
        // };
    
        
        using Name = std::pair<std::string, std::string>;
        
        void syntaxError(const std::string& message) const;
        
        // PARSING
        bool haveBeing() const;
        //bool haveDirection() const;
        bool have(Token::Kind kind) const;
        bool have(const std::set<Token::Kind>& kinds) const;
        bool have(const std::string& word) const;
        bool have(Grammar::Class wordClass) const;
        
        
        void matchBeing(const std::string& error = "invalid token");
        //void matchDirection(const std::string& error = "invalid token");
        void match(Token::Kind kind, const std::string& error = "invalid token");
        void match(const std::set<Token::Kind>& kinds, const std::string& error = "invalid token");
        void match(const std::string& word, const std::string& error = "invalid token");
        void match(Grammar::Class wordClass, const std::string& error = "invalid token");
        std::string eat();
        
        //void recRoom();
        
        void recDirectionDecl(Story& story);
        
        void recDecl(Story& story);
        Name recDeclStart(Story& story);
        
        void recRoomDecl(Story& story, Place& place);
        void recThingDecl(Story& story, Thing& thing);
        
        void recDirection(Story& story, Place& place);
        
        Action recAction(Story& story);
        // std::string recPosition();
        
        std::string recWords(const std::string& stop = "");
        std::string recWords(Grammar::Class stop);
        
        std::string text() const { return lex_.currentToken().text; }
        
        
        //std::map<std::string, Direction> directions_;
        Semantics sem_;
        Lexer lex_;
        const Grammar& grammar_;
    };
}

