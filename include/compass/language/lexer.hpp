//===--------------------------------------------------------------------------------------------===
// basiclexer.hpp - Basic (incomplete) lexer implementation re-used in the house engine
// This source is part of the Compass Engine
//
// Created on 2019-01-30 by Amy Parent <amy@amyparent.com>
// Copyright (c) 2019 Amy Parent
// Licensed under the MIT License
// =^•.•^=
//===--------------------------------------------------------------------------------------------===
#pragma once
#include <string>
#include <apfun/unicode.hpp>
#include <apfun/string.hpp>

namespace amyinorbit::compass {

    using cbuf = std::string;

    struct Token {
        enum Kind {
            Keyword,
            Integer,
            Word,
            QuotedString,
            Dash,
            Comma,
            Period,
            Amp,
            Colon,
            End,
        };

        Kind kind;
        string text;

        cbuf type() const;
    };

    class Lexer {
    public:
        Lexer(const string& source);
        virtual ~Lexer() {}

        void reset();
        virtual const Token& nextToken();
        const Token& currentToken() const;
    protected:


    private:
        unicode::scalar current() const;
        unicode::scalar nextChar();

        const Token& makeToken(Token::Kind kind, const string& str = "");

        bool isIdentifier(unicode::scalar c);
        void updateTokenStart();

        const Token& lexKeyword();
        const Token& lexString();
        const Token& lexWord();
        const Token& lexNumber();
        void eatLineComment();
        void eatParenComment();

        string source_ = "";

        unicode::scalar_iterator current_;
        unicode::scalar_iterator start_;
        Token currentToken_;
    };
}
