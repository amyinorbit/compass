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
#include <house/lexer.hpp>

namespace House {
    
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
        
        void haveWord(const std::string& word);
        void matchWord(const std::string& word, const std::string& error = "invalid token")
        
        void eat();
        
        void recAssetDesc();
        void recObjects();
        void recLinks();
        
        std::string recDetailedDesc();
        std::vector<std::string> recAdjectives();
        std::vector<std::string> recVerbs();
        std::string recWords(const std::set<Token::Kind>& stops);
        
        Lexer lexer;
    };
}

