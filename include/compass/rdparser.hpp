//===--------------------------------------------------------------------------------------------===
// rdparser.hpp - Recursive-descent parsing interface
// This source is part of the Compass Engine
//
// Created on 2019-02-05 by Amy Parent <amy@amyparent.com>
// Copyright (c) 2019 Amy Parent
// Licensed under the MIT License
// =^•.•^=
//===--------------------------------------------------------------------------------------------===
#pragma once
#include <string>
#include <compass/driver.hpp>
#include <compass/grammar.hpp>
#include <compass/lexer.hpp>
#include <compass/story.hpp>

namespace Compass {
    
    class RDParser {
    public:
        RDParser(const std::string& data, Driver& grammar);
        virtual ~RDParser() {}
    protected:
        
        bool haveBeing() const;
        bool have(Token::Kind kind) const;
        bool have(const std::string& word) const;
        bool have(Grammar::Class wordClass) const;
        
        bool matchBeing();
        bool match(Token::Kind kind);
        bool match(const std::string& word);
        bool match(Grammar::Class wordClass);
        
        void expectBeing(const std::string& error = "invalid token");
        void expect(Token::Kind kind, const std::string& error = "invalid token");
        void expect(const std::string& word, const std::string& error = "invalid token");
        void expect(Grammar::Class wordClass, const std::string& error = "invalid token");
        
        std::string recWords(const std::string& stop = "");
        std::string recWords(Grammar::Class stop);
        
        std::string eat();
        std::string text() const { return lexer.currentToken().text; }

        Lexer lexer;
        Driver& driver;
        
    private:
        void skipUntil(Token::Kind kind);
    };
}
