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
        : source_(source), iterator_(source_.unicode_scalars().begin()) {}

    void Lexer::reset() {
        iterator_ = source_.unicode_scalars().begin();
    }

    const Token& Lexer::currentToken() const {
        return currentToken_;
    }

    unicode::scalar Lexer::current() const {
        return *iterator_;
    }

    unicode::scalar Lexer::nextChar() {
        return *(++iterator_);
    }

    bool Lexer::isIdentifier(unicode::scalar c) {
        return unicode::is_identifier(c)
            || c == '-'
            || c == '\'';
    }

    void Lexer::updateTokenStart() {
        start_ = iterator_;
    }

    const Token& Lexer::lexKeyword() {
        while(isIdentifier(current())) {
            nextChar();
        }

        const auto str = string(start_.utf8(), iterator_.utf8());
        return makeToken(Token::Keyword, str);
    }

    const Token& Lexer::lexString() {

        while(current() != '"') {
            auto c = nextChar();
            if(c == '\0') {
                std::cerr << "quoted string not finished\n";
                abort();
            }
        }
        nextChar();

        const auto length = (iterator_.utf8() - start_.utf8())-2;
        return makeToken(Token::QuotedString, string(start_.utf8() + 1, length));
    }

    const Token& Lexer::lexWord() {
        while(isIdentifier(current())) {
            nextChar();
        }
        return makeToken(Token::Word, string(start_.utf8(), iterator_.utf8()));
    }

    const Token& Lexer::lexNumber() {
        while(current() >= '0' && current() < '9') {
            nextChar();
        }
        return makeToken(Token::Word, string(start_.utf8(), iterator_.utf8()));
    }

    const Token& Lexer::makeToken(Token::Kind kind, const string& str) {
        currentToken_.kind = kind;
        currentToken_.text = str;
        return currentToken_;
    }

    void Lexer::eatLineComment() {
        while(current().is_valid() && (current() != '\n' && current() != '\r')) {
            nextChar();
        }
    }

    void Lexer::eatParenComment() {
        while(current() != ')') nextChar();
        nextChar();
    }

    const Token& Lexer::nextToken() {
        if(iterator_ == source_.unicode_scalars().end()) {
            return makeToken(Token::End);
        }

        while(current() != '\0') {
            updateTokenStart();
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
                //     return makeToken(Token::Newline);

                case '.':
                    return makeToken(Token::Period);

                case ',':
                    return makeToken(Token::Comma);

                case '&':
                    return makeToken(Token::Amp);

                case '-':
                    return makeToken(Token::Dash);

                case '@':
                    return lexKeyword();

                case '"':
                    return lexString();

                case ':':
                    return makeToken(Token::Colon);

                case '#':
                case '!':
                    eatLineComment();
                    break;

                case '(':
                    eatParenComment();
                    break;

                default:
                    if(unicode::is_identifier_head(c)) {
                        return lexWord();
                    }
                    std::cerr << "Invalid character: "<< c <<"\n";
                    abort();
            }
        }
        return makeToken(Token::End);
    }
}
