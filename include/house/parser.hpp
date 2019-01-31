//===--------------------------------------------------------------------------------------------===
// parser.hpp - House adventure asset parser
// This source is part of the House Engine
//
// Created on 2019-01-30 by Amy Parent <amy@amyparent.com>
// Copyright (c) 2019 Amy Parent
// Licensed under the MIT License
// =^•.•^=
//===--------------------------------------------------------------------------------------------===
#pragma once
#include <string>
#include <set>
#include <house/utf8.h>

namespace House {
    struct Token {
        enum Kind {
            Objects,
            You,
            Links,
            Article,
            Descriptive,
            Word,
            Be,
            Can,
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
    };

    class AssetParser {
    public:
        AssetParser(const std::string& key, std::istream& stream);
        const std::string& key() const;
    
        void compile();
    
    private:
        
        // PARSING
        bool have(Token::Kind kind);
        bool have(const std::set<Token::Kind>& kinds);
        void match(Token::Kind kind, const std::string& error = "invalid token");
        void match(const std::set<Token::Kind>& kinds, const std::string& error = "invalid token");
        void eat();
        
        void recAssetDesc();
        void recObjects();
        void recLinks();
        
        std::string recDetailedDesc();
        std::vector<std::string> recAdjectives();
        std::vector<std::string> recVerbs();
        std::string recWords(const std::set<Token::Kind>& stops);
        
        //LEXING
        const Token& nextToken();
    
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

        std::string key_ = "";
        std::string file_ = "";
        
        std::size_t ptr_ = 0;
        std::size_t start_ = 0;
    
        codepoint_t current_;
        Token       currentToken_;
    };
}

