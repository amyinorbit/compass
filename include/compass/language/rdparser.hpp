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
#include <compass/language/driver.hpp>
#include <compass/language/grammar.hpp>
#include <compass/language/lexer.hpp>
#include <set>
#include <string>

namespace amyinorbit::compass {

    class RDParser {
    public:
        RDParser(const string& data, Driver& grammar);
        virtual ~RDParser() {}
    protected:

        bool have_being() const;
        bool have(Token::Kind kind) const;
        bool have(const string& word) const;
        bool have_any(const set<string>& words) const;
        bool have(Grammar::Class wordClass) const;

        bool match_being();
        bool match(Token::Kind kind);
        bool match(const string& word);
        bool match_any(const set<string>& words);
        bool match(Grammar::Class wordClass);

        void expect_being(const string& error = "invalid token");
        void expect(Token::Kind kind, const string& error = "invalid token");
        void expect(const string& word, const string& error = "invalid token");
        void expect_any(const set<string>& words, const string& error = "invalid token");
        void expect(Grammar::Class wordClass, const string& error = "invalid token");

        string words(const string& stop = "");
        string words_until_any(const set<string>& stop);
        string words(Grammar::Class stop);

        string eat();
        string text() const { return lexer.currentToken().text; }

        Lexer lexer;
        Driver& driver;

    private:
        void skip_until(Token::Kind kind);
    };
}
