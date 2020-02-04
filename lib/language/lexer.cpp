//===--------------------------------------------------------------------------------------------===
// lexer.cpp - Compass::Lexer lexing methods.
// This source is part of the Compass Engine
//
// Created on 2019-01-30 by Amy Parent <amy@amyparent.com>
// Copyright (c) 2019 Amy Parent
// Licensed under the MIT License
// =^•.•^=
//===--------------------------------------------------------------------------------------------===
#include <algorithm>
#include <cctype>
#include <iostream>
#include <cassert>
#include <vector>
#include <string>
#include <compass/language/lexer.hpp>

namespace amyinorbit::compass {

    cbuf tokenNames__[] = {
        "Keyword",
        "Integer",
        "Word",
        "QuotedString",
        "Dash",
        "Comma",
        "Period",
        "Amp",
        "Colon",
        "End",
    };

    cbuf Token::type() const {
        return tokenNames__[kind];
    }

    Lexer::Lexer(const string& source)
        : source_(source), current_(source_.unicode_scalars().begin()) {}

    void Lexer::reset() {
        current_ = source_.unicode_scalars().begin();
    }

    const Token& Lexer::current_token() const {
        return currentToken_;
    }

    unicode::scalar Lexer::current() const {
        return *current_;
    }

    unicode::scalar Lexer::nextChar() {
        return *(current_++);
    }

    bool Lexer::is_identifier(unicode::scalar c) {
        return c.is_identifier()
            || c == '-'
            || c == '\'';
    }

    // void Lexer::update_tokenStart() {
    //     start_ = current_;
    // }

    const Token& Lexer::lex_keyword() {
        while(is_identifier(current())) {
            nextChar();
        }

        const auto str = string(start_.utf8(), current_.utf8());
        return make_token(Token::Keyword, str);
    }

    const Token& Lexer::lex_string() {

        while(current() != '"') {
            auto c = nextChar();
            if(c == '\0') {
                std::cerr << "quoted string not finished\n";
                abort();
            }
        }
        nextChar();

        const auto length = (current_.utf8() - start_.utf8())-2;
        return make_token(Token::QuotedString, string(start_.utf8() + 1, length));
    }

    const Token& Lexer::lex_word() {
        while(is_identifier(current())) {
            nextChar();
        }
        return make_token(Token::Word, string(start_.utf8(), current_.utf8()));
    }

    const Token& Lexer::lex_number() {
        while(current() >= '0' && current() < '9') {
            nextChar();
        }
        return make_token(Token::Word, string(start_.utf8(), current_.utf8()));
    }

    const Token& Lexer::make_token(Token::Kind kind, const string& str) {
        currentToken_.kind = kind;
        currentToken_.text = str;
        // std::cout << "\t" << currentToken_.type() << "=" << str << "\n";
        return currentToken_;
    }

    void Lexer::eat_line_comment() {
        while(current().is_valid() && (current() != '\n' && current() != '\r')) {
            nextChar();
        }
    }

    void Lexer::eat_paren_comment() {
        while(current() != ')') nextChar();
        nextChar();
    }

    const Token& Lexer::next_token() {
        if(current_ == source_.unicode_scalars().end()) {
            return make_token(Token::End);
        }

        while(current().is_valid()) {
            start_ = current_;
            auto c = nextChar();

            switch(c.value) {
                //whitespace
                case 0x0020:
                case 0x000d:
                case 0x0009:
                case 0x000b:
                case 0x000c:
                case 0x000a:
                case 0xfeff:
                    break;

                // // line feed
                //     return make_token(Token::Newline);

                case '.':
                    return make_token(Token::Period);

                case ',':
                    return make_token(Token::Comma);

                case '&':
                    return make_token(Token::Amp);

                case '-':
                    return make_token(Token::Dash);

                case '@':
                    return lex_keyword();

                case '"':
                    return lex_string();

                case ':':
                    return make_token(Token::Colon);

                case '#':
                    eat_line_comment();
                    break;

                case '(':
                    eat_paren_comment();
                    break;

                default:
                    if(c.is_identifier_head()) {
                        return lex_word();
                    }
                    std::cerr << "Invalid character: "<< c <<"\n";
                    abort();
            }
        }
        return make_token(Token::End);
    }
}
