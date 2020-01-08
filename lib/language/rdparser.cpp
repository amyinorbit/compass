//===--------------------------------------------------------------------------------------------===
// rdparser.cpp - Implementation of recursive descent utilities
// This source is part of the Compass Engine
//
// Created on 2019-02-05 by Amy Parent <amy@amyparent.com>
// Copyright (c) 2019 Amy Parent
// Licensed under the MIT License
// =^•.•^=
//===--------------------------------------------------------------------------------------------===
#include <compass/language/driver.hpp>
#include <compass/language/rdparser.hpp>

namespace amyinorbit::compass {
    
    RDParser::RDParser(const string& data, Driver& driver)
        : lexer(data), driver(driver) {}

    bool RDParser::haveBeing() const {
        return driver.grammar().meansBeing(text());
    }
    
    bool RDParser::have(Token::Kind kind) const {
        return lexer.currentToken().kind == kind;
    }
    
    bool RDParser::have(const string& word) const {
        return text() == word;
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
    
    bool RDParser::match(const string& word) {
        if(!have(word)) return false;
        lexer.nextToken();
        return true;
    }
    
    bool RDParser::match(Grammar::Class wordClass) {
        if(!have(wordClass)) return false;
        lexer.nextToken();
        return true;
    }
    
    void RDParser::expectBeing(const cbuf& message) {
        if(driver.isFailed()) skipUntil(Token::End);
        if(!matchBeing()) driver.error(message);
    }

    void RDParser::expect(Token::Kind kind, const cbuf& message) {
        if(driver.isFailed()) skipUntil(kind);
        if(!match(kind)) driver.error(message);
    }
    
    void RDParser::expect(const string& word, const cbuf& message) {
        if(driver.isFailed()) skipUntil(Token::End);
        if(!match(word)) driver.error(message);
    }
    
    void RDParser::expect(Grammar::Class wordClass, const cbuf& message) {
        if(driver.isFailed()) skipUntil(Token::End);
        if(!match(wordClass)) driver.error(message);
    }
    
    string RDParser::eat() {
        string text = lexer.currentToken().text;
        lexer.nextToken();
        return text;
    }
    
    void RDParser::skipUntil(Token::Kind kind) {
        while(lexer.currentToken().kind != kind) {
            lexer.nextToken();
        }
        lexer.nextToken();
    }

    
    string RDParser::recWords(const string& stop) {
        auto str = text();
        match(Token::Word);
        while(have(Token::Word) && !haveBeing() && !have(stop)) str += " " + eat();
        return str;
    }
    
    string RDParser::recWords(Grammar::Class stop) {
        auto str = text();
        match(Token::Word);
        while(have(Token::Word) && !haveBeing() && !have(stop)) str += " " + eat();
        return str;
    }
}
