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

    void dump(const set<string>& words) {
        std::cout << "{";
        for(const auto& s: words) {
            std::cout << s << ",";
        }
        std::cout << "}\n";
    }

    RDParser::RDParser(const string& data, Driver& driver)
        : lexer(data), driver(driver) {}

    bool RDParser::have_being() const {
        return driver.grammar().means_being(text());
    }

    bool RDParser::have(Token::Kind kind) const {
        return lexer.current_token().kind == kind;
    }

    bool RDParser::have(const string& word) const {
        return text() == word;
    }

    bool RDParser::have_any(const set<string>& words) const {
        return words.count(text());
    }

    bool RDParser::have(Grammar::Class wordClass) const {
        return driver.grammar().is(text(), wordClass);
    }

    bool RDParser::match_being() {
        if(!have_being()) return false;
        lexer.next_token();
        return true;
    }

    bool RDParser::match_any(const set<string>& words) {
        if(!have_any(words)) return false;
        lexer.next_token();
        return true;
    }

    void RDParser::expect_being(const string& message) {
        if(is_recovering) {
            while(!have_being() && !have(Token::end)) {
                lexer.next_token();
            }
            if(have(Token::end)) return;
            lexer.next_token();
            is_recovering = false;
        } else {
            if(!match_being()) {
                driver.diagnostic(Diagnostic::error(message));
                is_recovering = true;
            }
        }
    }

    void RDParser::expect_any(const set<string>& words, const string& message) {
        if(is_recovering) {
            while(!have_any(words) && !have(Token::end)) {
                lexer.next_token();
            }
            if(have(Token::end)) return;
            lexer.next_token();
            is_recovering = false;
        } else {
            if(!match_any(words)) {
                driver.diagnostic(Diagnostic::error(message));
                is_recovering = true;
            }
        }
    }

    string RDParser::eat() {
        string text = lexer.current_token().text;
        lexer.next_token();
        return text;
    }

    void RDParser::skip_until(Token::Kind kind) {
        while(lexer.current_token().kind != kind) {
            if(lexer.current_token().kind == Token::Kind::end) return;
            lexer.next_token();
        }
        lexer.next_token();
    }

    string RDParser::words_until(const string& stop) {
        if(have(Token::quoted_string)) return eat();
        auto str = text();
        expect(Token::word);
        while(have(Token::word) && !have_being() && !have(stop)) {
            str += " ";
            str += eat();
        }
        return str;
    }

    string RDParser::words_until_any(const set<string>& stop) {
        if(have(Token::quoted_string)) return eat();
        auto str = text();
        expect(Token::word);
        while(have(Token::word) && !have_being() && !have_any(stop)) {
            str += " ";
            str += eat();
        }
        return str;
    }

    string RDParser::words_until(Grammar::Class stop) {
        if(have(Token::quoted_string)) return eat();
        auto str = text();
        expect(Token::word);
        while(have(Token::word) && !have_being() && !have(stop)) {
            str += " ";
            str += eat();
        }
        return str;
    }
}
