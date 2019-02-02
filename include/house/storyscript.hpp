//===--------------------------------------------------------------------------------------------===
// parser.hpp - House adventure asset parser
// This source is part of the House Engine
//
// Created on 2019-01-30 by Amy Parent <amy@amyparent.com>
// Copyright (c) 2019 Amy Parent
// Licensed under the MIT License
// =^•.•^=
//===--------------------------------------------------------------------------------------------===
#pragma once
#include <string>
#include <set>
#include <house/asset.hpp>
#include <house/grammar.hpp>
#include <house/lexer.hpp>

namespace House {
    
    class StoryParser {
    public:
        StoryParser(const std::string& data, const Grammar& grammar);
        void compile();
    
    private:
        
        using Name = std::pair<std::string, std::string>;
        
        void syntaxError(const std::string& message) const;
        
        // PARSING
        bool haveBeing() const;
        bool have(Token::Kind kind) const;
        bool have(const std::set<Token::Kind>& kinds) const;
        bool have(const std::string& word) const;
        bool have(Grammar::Class wordClass) const;
        
        void matchBeing(const std::string& error = "invalid token");
        void match(Token::Kind kind, const std::string& error = "invalid token");
        void match(const std::set<Token::Kind>& kinds, const std::string& error = "invalid token");
        void match(const std::string& word, const std::string& error = "invalid token");
        void match(Grammar::Class wordClass, const std::string& error = "invalid token");
        std::string eat();
        
        //void recRoom();
        std::string recPosition();
        
        void recDecl();
        Name recDeclStart();
        
        void recRoomDecl(Name name);
        void recThingDecl(Name name);
        
        
        std::string recWords(const std::string& stop = "");
        
        std::string text() const { return lex_.currentToken().text; }
        Lexer lex_;
        const Grammar& grammar_;
    };
}

