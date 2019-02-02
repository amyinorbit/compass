//===--------------------------------------------------------------------------------------------===
// storyscriot.cpp - Implementation of the House Engine's story script file parser
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
#include <house/storyscript.hpp>

namespace House {
    
    void StoryParser::compile() {
        eat();
        // TODO: parse by sections, verbs, story.
        while(!have(Token::End)) {
            recDecl();
        }
        match(Token::End);
        
    }
    
    void StoryParser::recDecl() {
        auto name = recDeclStart();
        matchBeing("invalid verb after a thing or place");
        
        if(have(Grammar::Definite) || have(Grammar::Indefinite)) {
            eat();
            recRoomDecl(name);
        }
        else if(have(Grammar::Preposition)) {
            recThingDecl(name);
        }
        else {
            syntaxError("cannot infer what '" + name.second + "' is");
        }
    }
    
    StoryParser::Name StoryParser::recDeclStart() {
        std::string article, name;
        
        if(have(Grammar::Definite) || have(Grammar::Indefinite)) article = eat();
        if(haveBeing()) syntaxError("things and places must have names");
        
        name = recWords();
        return std::make_pair(article, name);
    }
    
    void StoryParser::recRoomDecl(Name name) {
        if(have("room")) eat();
        else if(have("place")) eat();
        else syntaxError("this should be a place, but I can't see 'room' or 'place'");
        
        if(!have(Token::Period)) {
            // TODO: parse relative location
            // TODO: remove this bad placeholder
            while(!have(Token::Period)) eat();
        }
        
        match(Token::Period);
        
        std::string description = text();
        match(Token::QuotedString);
        
        std::cout << "---room:\n";
        std::cout << "   " << name.second << " (" << name.first << ")\n";
        std::cout << "   " << description << "\n\n";
    }
    
    void StoryParser::recThingDecl(Name name) {
        Name room = std::make_pair("", "");
        std::vector<std::string> adjectives;
        std::string rel = text();
        std::string description;
        std::string details;
        
        match(Grammar::Preposition, "Things must be placed somehwere");
        // Parse the location of the object.
        if(have(Grammar::Definite) || have(Grammar::Indefinite)) room.first = eat();
        room.second = recWords();
        
        // adjectives?
        while(have(Token::Comma) || have("and")) {
            eat();
            adjectives.push_back(recWords("and"));
        }
        
        if(have(Token::Colon)) {
            eat();
            description = text();
            match(Token::QuotedString, "descriptions of short things should be in quotes");
        }
        match(Token::Period);

        
        std::cout << "---thing:\n";
        std::cout << "   ";
        for(const auto& adj: adjectives)
            std::cout << adj << " ";
        std::cout << "\n";
        std::cout << "   " << name.second << " (" << name.first << ")\n";
        std::cout << "   " << description << "/" << details << "\n\n";
    }
    
    std::string StoryParser::recPosition() {
        // TODO: match cardinal directions
        // if(haveCardinal()) {}
        // if(have)
        std::string relative = text();
        match(Token::Word);
        if(have(Grammar::Preposition)) eat();
        
        if(have(Grammar::Definite)) eat();
        std::string name = text();
        match(Token::Word);
        while(have(Token::Word)) {
            name += " " + eat();
        }
        return relative + ", " + name;
    }
    
    std::string StoryParser::recWords(const std::string& stop) {
        std::string str = text();
        match(Token::Word);
        while(have(Token::Word) && !haveBeing() && !have(stop)) str += " " + eat();
        return str;
    }
}
