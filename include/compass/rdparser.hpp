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
#include <compass/grammar.hpp>
#include <compass/lexer.hpp>
#include <compass/story.hpp>

namespace Compass {
    class RDParser {
    public:
        RDParser(const std::string& data, const Grammar& grammar);
        virtual ~RDParser() {}
    protected:
        virtual void error(const std::string& message) = 0;
        
        void fail() { failed_ = true; }
        bool isFailed() const { return failed_; }
        
        bool haveBeing() const;
        bool have(Token::Kind kind) const;
        bool have(const std::string& word) const;
        bool have(Grammar::Class wordClass) const;
        
        void matchBeing(const std::string& error = "invalid token");
        void match(Token::Kind kind, const std::string& error = "invalid token");
        void match(const std::string& word, const std::string& error = "invalid token");
        void match(Grammar::Class wordClass, const std::string& error = "invalid token");
        
        std::string recWords(const std::string& stop = "");
        std::string recWords(Grammar::Class stop);
        
        std::string eat();
        std::string text() const { return lexer.currentToken().text; }

        Lexer lexer;
        const Grammar& grammar;
        
    private:
        void skipUntil(Token::Kind kind);
        bool failed_;
    };
}
