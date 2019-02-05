//===--------------------------------------------------------------------------------------------===
// storyscript+rd.cpp - Implementation of the Compass Engine's story script file parser's RD utilities
// This source is part of the Compass Engine
//
// Created on 2019-01-30 by Amy Parent <amy@amyparent.com>
// Copyright (c) 2019 Amy Parent
// Licensed under the MIT License
// =^•.•^=
//===--------------------------------------------------------------------------------------------===
#include <iostream>
#include <vector>
#include <string>
#include <compass/storyparser.hpp>

namespace Compass {
    
    static std::string toLower(const std::string& str) {
        std::string lower = str;
        std::transform(lower.begin(), lower.end(), lower.begin(), ::tolower);
        return lower;
    }
    
    StoryParser::StoryParser(const std::string& data, const Grammar& grammar)
        : lex_(data), grammar_(grammar) {}


    void StoryParser::syntaxError(const std::string& message) const {
        std::cerr << "error: " << message << "\n";
        std::cerr << "token: " << text() << "(" << lex_.currentToken().type() << ")\n";
        abort();
    }

    bool StoryParser::haveBeing() const {
        return grammar_.meansBeing(text());
    }
    
    bool StoryParser::have(Token::Kind kind) const {
        return lex_.currentToken().kind == kind;
    }
    
    bool StoryParser::have(const std::set<Token::Kind>& kinds) const {
        return kinds.find(lex_.currentToken().kind) != kinds.end();
    }
    
    bool StoryParser::have(const std::string& word) const {
        return toLower(text()) == toLower(word);
    }
    
    bool StoryParser::have(Grammar::Class wordClass) const {
        return grammar_.is(text(), wordClass);
    }
    
    void StoryParser::matchBeing(const std::string& error) {
        if(!haveBeing()) syntaxError(error);
        lex_.nextToken();
    }

    void StoryParser::match(Token::Kind kind, const std::string& error) {
        if(!have(kind)) syntaxError(error);
        lex_.nextToken();
    }

    void StoryParser::match(const std::set<Token::Kind>& kinds, const std::string& error) {
        if(!have(kinds)) syntaxError(error);
        lex_.nextToken();
    }
    
    void StoryParser::match(const std::string& word, const std::string& error) {
        if(!have(word)) syntaxError(error);
        lex_.nextToken();
    }
    
    void StoryParser::match(Grammar::Class wordClass, const std::string& error) {
        if(!have(wordClass)) syntaxError(error);
        lex_.nextToken();
    }
    
    std::string StoryParser::eat() {
        std::string text = lex_.currentToken().text;
        lex_.nextToken();
        return text;
    }
}
