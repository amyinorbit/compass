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
#include <house/assetparser.hpp>

namespace House {
    
    static std::string toLower(const std::string& str) {
        std::string lower = str;
        std::transform(lower.begin(), lower.end(), lower.begin(), ::tolower);
        return lower;
    }
    
    AssetParser::AssetParser(const std::string& data, const Grammar& grammar)
        : lex_(data), grammar_(grammar) {}


    void AssetParser::syntaxError(const std::string& message) const {
        std::cerr << "error: " << message << "\n";
        abort();
    }

    bool AssetParser::haveBeing() const {
        return grammar_.meansBeing(lex_.currentToken().text);
    }
    
    bool AssetParser::have(Token::Kind kind) const {
        return lex_.currentToken().kind == kind;
    }
    
    bool AssetParser::have(const std::set<Token::Kind>& kinds) const {
        return kinds.find(lex_.currentToken().kind) != kinds.end();
    }
    
    bool AssetParser::have(const std::string& word) const {
        return toLower(lex_.currentToken().text) == toLower(word);
    }
    
    bool AssetParser::have(Grammar::Class wordClass) const {
        return grammar_.is(lex_.currentToken().text, wordClass);
    }
    
    void AssetParser::matchBeing(const std::string& error) {
        if(!haveBeing()) syntaxError(error);
        lex_.nextToken();
    }

    void AssetParser::match(Token::Kind kind, const std::string& error) {
        if(!have(kind)) syntaxError(error);
        lex_.nextToken();
    }

    void AssetParser::match(const std::set<Token::Kind>& kinds, const std::string& error) {
        if(!have(kinds)) syntaxError(error);
        lex_.nextToken();
    }
    
    void AssetParser::match(const std::string& word, const std::string& error) {
        if(!have(word)) syntaxError(error);
        lex_.nextToken();
    }
    
    void AssetParser::match(Grammar::Class wordClass, const std::string& error) {
        if(!have(wordClass)) syntaxError(error);
        lex_.nextToken();
    }
    
    void AssetParser::eat() {
        lex_.nextToken();
    }
    
    House::Room AssetParser::compile() {
        eat();
        auto asset = recAssetDesc();
        
        if(have("objects")) {
            match("objects");
            match(Token::Colon);
            while(have(Token::Dash))
                asset.objects.push_back(recObject());
        }
        
        if(have("around")) {
            match("around");
            match(Token::Colon);
            while(have(Token::Dash))
                asset.links.push_back(recLink());
        }
        return asset;
    }

    House::Room AssetParser::recAssetDesc() {
        match("this", "missing subject in asset description");
        matchBeing();
        
        House::Room room;

        if(have(Grammar::Definite) || have(Grammar::Indefinite)) {
            room.article = lex_.currentToken().text;
            eat();
        }

        room.name = recWords();
        match(Token::Period, "Asset description must end with a period");
        std::cout << "This is: " << room.name << "\n";
        
        if(have(Grammar::Subjective)) {
            room.description = recDetailedDesc();
            std::cout << " > " << room.description << "\n";
        }
        
        return room;
    }

    House::Object AssetParser::recObject() {
        House::Object obj;
        
        match(Token::Dash);
        obj.article = lex_.currentToken().text;
        match(Grammar::Indefinite);
        obj.name = recWords();
        obj.adjectives = recAdjectives();
        match(Token::Period);
        
        for(const auto& adj: obj.adjectives) {
            std::cout << adj << " ";
        }
        std::cout << obj.name << "\n";

        if(have("you")) {
            obj.verbs = recVerbs();
            std::cout << "you can ";
            for(const auto& v: obj.verbs) {
                std::cout << v << " it,";
                
            }
            std::cout << ".\n";
        }

        if(have(Grammar::Subjective)) {
            obj.description = recDetailedDesc();
            std::cout << " > " << obj.description << "\n";
        }
        
        return obj;
    }
    
    House::Link AssetParser::recLink() {
        House::Link link;
        
        match(Token::Dash);
        
        if(have(Grammar::Indefinite) || have(Grammar::Definite)) {
            link.article = lex_.currentToken().text;
            eat();
        }
        link.name = recWords("is");
        matchBeing("other rooms must be situated around this one");
        link.direction = recDirection();
        
        std::cout << link.name << " is " << link.direction << "\n";
        match(Token::Period);
        
        if(have(Grammar::Subjective)) {
            link.description = recDetailedDesc();
            std::cout << " > " << link.description << "\n";
        }
        return link;
    }
    
    std::string AssetParser::recDirection() {
        auto dir = lex_.currentToken().text;
        match(Token::Word);
        return dir;
    }
    
    std::string AssetParser::recDetailedDesc() {
        match(Grammar::Subjective, "detailedDescriptions must start with a subject");
        matchBeing();
        const auto str = lex_.currentToken().text;
        match(Token::QuotedString);
        match(Token::Period);
        return str;
    }

    std::vector<std::string> AssetParser::recAdjectives() {
        std::vector<std::string> adjectives;
        while(have(Token::Comma) || have(Token::Amp) || have("and")) {
            eat();
            adjectives.push_back(recWords());
        }
        return adjectives;
    }

    std::vector<std::string> AssetParser::recVerbs() {
        std::vector<std::string> verbs;
        match("you");
        match("can");
        verbs.push_back(recVerb());
        
        while(have(Token::Comma) || have(Token::Amp) || have("and")) {
            eat();
            verbs.push_back(recVerb());
        }

        match(Token::Period);
        return verbs;
    }
    
    std::string AssetParser::recVerb() {
        auto verb = lex_.currentToken().text;
        match(Token::Word);
        if(have(Grammar::Preposition)) {
            verb += " " + lex_.currentToken().text;
            eat();
        }
        match(Grammar::Objective);
        return verb;
    }

    // Basic chunks recognisers
    std::string AssetParser::recWords(const std::string& stop) {
        std::string str = "";
        while(have(Token::Word) && !have(stop)) {
            str += " " + lex_.currentToken().text;
            eat();
        }
        return str;
    }
}
