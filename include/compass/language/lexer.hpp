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
            LParen,
            RParen,
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
        virtual const Token& next_token();
        const Token& current_token() const;
    protected:


    private:
        unicode::scalar current() const;
        unicode::scalar nextChar();

        const Token& make_token(Token::Kind kind, const string& str = "");

        bool is_identifier(unicode::scalar c);
        // void updateTokenStart();

        const Token& lex_keyword();
        const Token& lex_string();
        const Token& lex_word();
        const Token& lex_number();
        void eat_line_comment();
        void eat_paren_comment();

        string source_ = "";

        unicode::scalar_iterator current_;
        unicode::scalar_iterator start_;
        Token currentToken_;
    };
}
