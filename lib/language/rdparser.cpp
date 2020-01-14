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

    bool RDParser::have_being() const {
        return driver.grammar().meansBeing(text());
    }

    bool RDParser::have(Token::Kind kind) const {
        return lexer.currentToken().kind == kind;
    }

    bool RDParser::have(const string& word) const {
        return text() == word;
    }

    bool RDParser::have_any(const std::set<string>& words) const {
        return words.count(text()) > 0;
    }

    bool RDParser::have(Grammar::Class wordClass) const {
        return driver.grammar().is(text(), wordClass);
    }

    bool RDParser::match_being() {
        if(!have_being()) return false;
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

    bool RDParser::match_any(const std::set<string>& words) {
        if(!have_any(words)) return false;
        lexer.nextToken();
        return true;
    }

    bool RDParser::match(Grammar::Class wordClass) {
        if(!have(wordClass)) return false;
        lexer.nextToken();
        return true;
    }

    void RDParser::expect_being(const string& message) {
        if(driver.isFailed()) skip_until(Token::End);
        if(!match_being()) driver.diagnostic(Diagnostic::error(message));
    }

    void RDParser::expect(Token::Kind kind, const string& message) {
        if(driver.isFailed()) skip_until(kind);
        if(!match(kind)) driver.diagnostic(Diagnostic::error(message));
    }

    void RDParser::expect(const string& word, const string& message) {
        if(driver.isFailed()) skip_until(Token::End);
        if(!match(word)) driver.diagnostic(Diagnostic::error(message));
    }

    void RDParser::expect_any(const std::set<string>& words, const string& message) {
        if(driver.isFailed()) skip_until(Token::End);
        if(!match_any(words)) driver.diagnostic(Diagnostic::error(message));
    }

    void RDParser::expect(Grammar::Class wordClass, const string& message) {
        if(driver.isFailed()) skip_until(Token::End);
        if(!match(wordClass)) driver.diagnostic(Diagnostic::error(message));
    }

    string RDParser::eat() {
        string text = lexer.currentToken().text;
        lexer.nextToken();
        return text;
    }

    void RDParser::skip_until(Token::Kind kind) {
        while(lexer.currentToken().kind != kind) {
            lexer.nextToken();
        }
        lexer.nextToken();
    }

    string RDParser::words(const string& stop) {
        auto str = text();
        match(Token::Word);
        while(have(Token::Word) && !have_being() && !have(stop)) str += " " + eat();
        return str;
    }

    string RDParser::words_until_any(const std::set<string>& stop) {
        auto str = text();
        match(Token::Word);
        while(have(Token::Word) && !have_being() && !have_any(stop)) str += " " + eat();
        return str;
    }

    string RDParser::words(Grammar::Class stop) {
        auto str = text();
        match(Token::Word);
        while(have(Token::Word) && !have_being() && !have(stop)) str += " " + eat();
        return str;
    }
}
