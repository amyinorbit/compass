//===--------------------------------------------------------------------------------------------===
// rdparser.cpp - Implementation of recursive descent utilities
// This source is part of the Compass Engine
//
// Created on 2019-02-05 by Amy Parent <amy@amyparent.com>
// Copyright (c) 2019 Amy Parent
// Licensed under the MIT License
// =^•.•^=
//===--------------------------------------------------------------------------------------------===
#include <compass/rdparser.hpp>

namespace Compass {
    
    static std::string toLower(const std::string& str) {
        std::string lower = str;
        for(auto& c: lower) c = ::tolower(c);
        return lower;
    }
    
    RDParser::RDParser(const std::string& data, const Grammar& grammar)
        : failed_(false), lexer(data), grammar(grammar) {}

    bool RDParser::haveBeing() const {
        return grammar.meansBeing(text());
    }
    
    bool RDParser::have(Token::Kind kind) const {
        return lexer.currentToken().kind == kind;
    }
    
    bool RDParser::have(const std::string& word) const {
        return toLower(text()) == toLower(word);
    }
    
    bool RDParser::have(Grammar::Class wordClass) const {
        return grammar.is(text(), wordClass);
    }
    
    void RDParser::matchBeing(const std::string& message) {
        if(failed_) skipUntil(Token::End);
        if(!haveBeing()) error(message);
        lexer.nextToken();
    }

    void RDParser::match(Token::Kind kind, const std::string& message) {
        if(failed_) skipUntil(kind);
        if(!have(kind)) error(message);
        lexer.nextToken();
    }
    
    void RDParser::match(const std::string& word, const std::string& message) {
        if(failed_) skipUntil(Token::End);
        if(!have(word)) error(message);
        lexer.nextToken();
    }
    
    void RDParser::match(Grammar::Class wordClass, const std::string& message) {
        if(failed_) skipUntil(Token::End);
        if(!have(wordClass)) error(message);
        lexer.nextToken();
    }
    
    std::string RDParser::eat() {
        std::string text = lexer.currentToken().text;
        lexer.nextToken();
        return text;
    }
    
    void RDParser::skipUntil(Token::Kind kind) {
        while(lexer.currentToken().kind != kind) {
            lexer.nextToken();
        }
        lexer.nextToken();
    }

    
    std::string RDParser::recWords(const std::string& stop) {
        std::string str = text();
        match(Token::Word);
        while(have(Token::Word) && !haveBeing() && !have(stop)) str += " " + eat();
        return str;
    }
    
    std::string RDParser::recWords(Grammar::Class stop) {
        std::string str = text();
        match(Token::Word);
        while(have(Token::Word) && !haveBeing() && !have(stop)) str += " " + eat();
        return str;
    }
}
