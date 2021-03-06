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

        template <typename T>
        bool match(const T& value) {
            if(!have(value)) return false;
            lexer.next_token();
            return true;
        }
        bool match_being();
        bool match_any(const set<string>& words);

        template <typename T>
        void expect(const T& value, const string& error = "invalid token");
        void expect_being(const string& error = "invalid token");
        void expect_any(const set<string>& words, const string& error = "invalid token");

        string words_until(const string& stop = "");
        string words_until_any(const set<string>& stop);
        string words_until(Grammar::Class stop);

        string eat();
        string text() const { return lexer.current_token().text; }

        void error(const string& error) {
            driver.diagnostic(Diagnostic::error(error));
        }

        Lexer lexer;
        Driver& driver;

    private:
        bool is_recovering = false;
        void skip_until(Token::Kind kind);
    };

    template <typename T>
    inline void RDParser::expect(const T& value, const string& message) {
        if(is_recovering) {
            while(!have(value) && !have(Token::end)) {
                lexer.next_token();
            }
            if(have(Token::end)) return;
            lexer.next_token();
            is_recovering = false;
        } else {
            if(!match(value)) {
                driver.diagnostic(Diagnostic::error(message));
                is_recovering = true;
            }
        }
    }
}
