//===--------------------------------------------------------------------------------------------===
// storyscriot.cpp - Implementation of the House Engine's story script file parser
// This source is part of the House Engine
//
// Created on 2019-01-30 by Amy Parent <amy@amyparent.com>
// Copyright (c) 2019 Amy Parent
// Licensed under the MIT License
// =^•.•^=
//===--------------------------------------------------------------------------------------------===
#include <iostream>
#include <vector>
#include <string>
#include <house/storyscript.hpp>

namespace House {
    
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
        return grammar_.meansBeing(lex_.currentToken().text);
    }
    
    bool StoryParser::have(Token::Kind kind) const {
        return lex_.currentToken().kind == kind;
    }
    
    bool StoryParser::have(const std::set<Token::Kind>& kinds) const {
        return kinds.find(lex_.currentToken().kind) != kinds.end();
    }
    
    bool StoryParser::have(const std::string& word) const {
        return toLower(lex_.currentToken().text) == toLower(word);
    }
    
    bool StoryParser::have(Grammar::Class wordClass) const {
        return grammar_.is(lex_.currentToken().text, wordClass);
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
    
    
    void StoryParser::compile() {
        eat();
        // TODO: parse by sections, verbs, story.
        while(!have(Token::End)) {
            recRoom();
        }
        match(Token::End);
        
    }
    
    void StoryParser::recRoom() {
        std::string article;
        std::string description;
        std::string relative;
        
        if(have(Grammar::Definite)) {
            article = text();
            eat();
        }
        
        std::string name = text();
        match(Token::Word); // TODO: we need a better way to do things here
        while(have(Token::Word) && !haveBeing()) {
            name += " " + eat();
        }
        
        matchBeing(name + " must *be* a thing or a place");
        match("a", name + " must be *a* thing or a place");
        match("place", name + " must be a *thing or a place*");
        
        if(!have(Token::Period)) {
            relative = recPosition();
        }
        match(Token::Period);
        
        if(have(Token::QuotedString)) {
            description = eat();
        }
        if(have(Token::Period)) eat();
        
        std::cout << "* " << name << "(" << article << ")" "\n";
        std::cout << " > " << relative << "\n";
        std::cout << " > " << description << "\n\n";
    }
    
    std::string StoryParser::recPosition() {
        // TODO: match cardinal directions
        // if(haveCardinal()) {}
        // if(have)
        std::string relative = text();
        match(Token::Word);
        if(have(Grammar::Preposition)) eat();
        
        if(have(Grammar::Definite)) eat();
        std::string name = text();
        match(Token::Word);
        while(have(Token::Word)) {
            name += " " + eat();
        }
        return relative + ", " + name;
    }
}
