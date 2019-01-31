//===--------------------------------------------------------------------------------------------===
// parser.cpp - Implementation of the House Engine's asset/room file parser
// This source is part of the House Engine
//
// Created on 2019-01-30 by Amy Parent <amy@amyparent.com>
// Copyright (c) 2019 Amy Parent
// Licensed under the MIT License
// =^•.•^=
//===--------------------------------------------------------------------------------------------===
#include <iostream>
#include <vector>
#include <string>
#include <house/parser.hpp>

static std::string names[] = {
    
    "kw_objects",
    "kw_you",
    "kw_links",
    "en_article",
    "en_descr",
    "word",
    "verb_be",
    "verb_can",
    "lit_string",
    "punc_dash",
    "punc_comma",
    "punc_period",
    "punc_amp",
    "punc_colon",
    "file_end"
};

namespace House {
    AssetParser::AssetParser(const std::string& key, std::istream& stream) {
        key_ = key;
        file_ = std::string(std::istreambuf_iterator<char>(stream), {});
    }
    
    bool AssetParser::have(Token::Kind kind) {
        return currentToken_.kind == kind;
    }
    
    bool AssetParser::have(const std::set<Token::Kind>& kinds) {
        return kinds.find(currentToken_.kind) != kinds.end();
    }

    void AssetParser::match(Token::Kind kind, const std::string& error) {
        if(have(kind)) {
            nextToken();
            return;
        }
        std::cerr << "error in file '" << key_ << "': " << error << "\n";
        std::cerr << "token: [" << names[currentToken_.kind] << "] " << currentToken_.text << "\n";
        std::cerr << "expected: [" << names[kind] << "]\n";
        abort();
    }

    void AssetParser::match(const std::set<Token::Kind>& kinds, const std::string& error) {
        if(have(kinds)) {
            nextToken();
            return;
        }
        std::cerr << "error in file '" << key_ << "': " << error << "\n";
        std::cerr << "token: [" << names[currentToken_.kind] << "] " << currentToken_.text << "\n";
        std::cerr << "expected: [";
        for (auto k: kinds) {
            std::cerr << names[k] << ",";
        }
        std::cerr << "]\n";
        abort();
    }
    
    void AssetParser::compile() {
        nextToken();
        recAssetDesc();
        if(have(Token::Objects)) {
            recObjects();
        }
    }
        
    void AssetParser::recAssetDesc() {
        match(Token::Descriptive, "missing subject in asset description");
        match(Token::Be, "missing verb in asset description");
        
        std::string article = "";
        std::string name = "";
        
        if(have(Token::Article)) {
            article  = currentToken_.text;
            match(Token::Article, "PROGRAMMER ERROR");
        }
        
        name = recWords({Token::Period});
        match(Token::Period, "Asset description must end with a period");
        std::cout << "This is: " << name << " (" << article << ")\n";
        
        if(have(Token::Descriptive)) {
            const auto desc = recDetailedDesc();
            std::cout << " > " << desc << "\n";
        }
    }
    
    void AssetParser::recObjects() {
        match(Token::Objects);
        match(Token::Colon);
        
        while(have(Token::Dash)) {
            match(Token::Dash);
            
            const auto article = currentToken_.text;
            match(Token::Article);
            auto name = recWords({Token::Comma, Token::Period});
            auto adjectives = recAdjectives();
            match(Token::Period);
            
            std::cout << article << " ";
            for(const auto& adj: adjectives) {
                std::cout << adj << " ";
            }
            std::cout << name << "\n";
            
            if(have(Token::You)) {
                const auto verbs = recVerbs();
                std::cout << "you can ";
                for(const auto& v: verbs)
                    std::cout << v << " it, ";
                std::cout << ".";
            }
            
            if(have(Token::Descriptive)) {
                const auto desc = recDetailedDesc();
                std::cout << " > " << desc << "\n";
            }
        }
    }
    
    void AssetParser::recLinks() {
        
    }
    
    std::string AssetParser::recDetailedDesc() {
        match(Token::Descriptive, "detailedDescriptions must start with a subject");
        match(Token::Be);
        const auto str = currentToken_.text;
        match(Token::QuotedString);
        match(Token::Period);
        return str;
    }

    std::vector<std::string> AssetParser::recAdjectives() {
        std::vector<std::string> adjectives;
        while(have({Token::Comma, Token::Amp})) {
            match({Token::Comma, Token::Amp});
            adjectives.push_back(recWords({Token::Comma, Token::Period}));
        }
        return adjectives;
    }
    
    std::vector<std::string> AssetParser::recVerbs() {
        std::vector<std::string> verbs;
        match(Token::You);
        match(Token::Can);
        verbs.push_back(currentToken_.text);
        match(Token::Word);
        //if(have(Token::Preposition)) match(Token::Preposition); 
        match(Token::Descriptive);// TODO: pronoun instead
        
        while(have({Token::Comma, Token::Amp})) {
            match({Token::Comma, Token::Amp});
            verbs.push_back(currentToken_.text);
            match(Token::Word);
            match(Token::Descriptive);// TODO: pronoun instead
        }
        
        match(Token::Period);
        return verbs;
    }
    
    // Basic chunks recognisers
    std::string AssetParser::recWords(const std::set<Token::Kind>& stops) {
        std::string str = currentToken_.text;
        match({Token::Word, Token::Be, Token::Article});
        while(!have(stops)) {
            str += " " + currentToken_.text;
            match({Token::Word, Token::Be, Token::Article});
        }
        return str;
    }
}
