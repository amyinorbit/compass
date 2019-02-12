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
#include <compass/utf8.h>

namespace Compass {
    
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
        std::string text;
        
        std::string type() const;
    };
    
    class Lexer {
    public:
        Lexer(const std::string& source);
        virtual ~Lexer() {}
        
        void reset();
        virtual const Token& nextToken();
        const Token& currentToken() const;
    protected:
        
        
    private:
        codepoint_t current() const;
        codepoint_t nextChar();
        std::size_t remaining() const;
    
        const Token& makeToken(Token::Kind kind, const std::string& str = "");
    
        bool isIdentifier(codepoint_t c);
        void updateTokenStart();
        
        const Token& lexKeyword();
        const Token& lexString();
        const Token& lexWord();
        const Token& lexNumber();
        void eatLineComment();
        void eatParenComment();
        
        std::string source_ = "";
        
        std::size_t ptr_ = 0;
        std::size_t start_ = 0;
    
        codepoint_t current_;
        Token       currentToken_;
    };
}