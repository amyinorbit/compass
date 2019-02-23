//===--------------------------------------------------------------------------------------------===
// rdparser.cpp - Implementation of recursive descent utilities
// This source is part of the Compass Engine
//
// Created on 2019-02-05 by Amy Parent <amy@amyparent.com>
// Copyright (c) 2019 Amy Parent
// Licensed under the MIT License
// =^•.•^=
//===--------------------------------------------------------------------------------------------===
#include <compass/core/string.hpp>
#include <compass/language/driver.hpp>
#include <compass/language/rdparser.hpp>

namespace Compass::Language {
    
    RDParser::RDParser(const std::string& data, Driver& driver)
        : lexer(data), driver(driver) {}

    bool RDParser::haveBeing() const {
        return driver.grammar().meansBeing(text());
    }
    
    bool RDParser::have(Token::Kind kind) const {
        return lexer.currentToken().kind == kind;
    }
    
    bool RDParser::have(const std::string& word) const {
        return String::toLower(text()) == String::toLower(word);
    }
    
    bool RDParser::have(Grammar::Class wordClass) const {
        return driver.grammar().is(text(), wordClass);
    }
    
    bool RDParser::matchBeing() {
        if(!haveBeing()) return false;
        lexer.nextToken();
        return true;
    }
    
    bool RDParser::match(Token::Kind kind) {
        if(!have(kind)) return false;
        lexer.nextToken();
        return true;
    }
    
    bool RDParser::match(const std::string& word) {
        if(!have(word)) return false;
        lexer.nextToken();
        return true;
    }
    
    bool RDParser::match(Grammar::Class wordClass) {
        if(!have(wordClass)) return false;
        lexer.nextToken();
        return true;
    }
    
    void RDParser::expectBeing(const std::string& message) {
        if(driver.isFailed()) skipUntil(Token::End);
        if(!matchBeing()) driver.error(message);
    }

    void RDParser::expect(Token::Kind kind, const std::string& message) {
        if(driver.isFailed()) skipUntil(kind);
        if(!match(kind)) driver.error(message);
    }
    
    void RDParser::expect(const std::string& word, const std::string& message) {
        if(driver.isFailed()) skipUntil(Token::End);
        if(!match(word)) driver.error(message);
    }
    
    void RDParser::expect(Grammar::Class wordClass, const std::string& message) {
        if(driver.isFailed()) skipUntil(Token::End);
        if(!match(wordClass)) driver.error(message);
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
