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
#include <house/story.hpp>

namespace House {
    
    void StoryParser::compile() {
        eat();
        
        if(have("directions")) {
            eat();
            match(Token::Colon);
            
            while(!have("story")) {
                recDirectionDecl();
            }
        }
        match("story");
        match(Token::Colon);
        
        while(!have(Token::End)) {
            recDecl();
        }
        match(Token::End);
        
    }
    
    void StoryParser::recDirectionDecl() {
        Direction dir;
        dir.direction = recWords();
        matchBeing();
        match(Grammar::Indefinite, "directions must be *a* direction");
        match("direction", "I expected a direction");
        
        if(have(Token::Comma) || have("and")) {
            eat();
            match(Grammar::Definite, "expected a 'the' before opposite");
            match("opposite", "expected 'opposite'");
            matchBeing();
            dir.opposite = recWords();
        }
        match(Token::Period);
        
        directions_[dir.direction] = dir;
        if(dir.opposite.size()) {
            Direction opp{dir.opposite, dir.direction};
            directions_[opp.direction] = opp;
        }
        std::cout << dir.direction << " <-> " << dir.opposite << "\n";
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
            std::string loc = recWords(Grammar::Preposition);
            match(Grammar::Preposition);
            std::string article, room;
            
            if(have(Grammar::Definite) || have(Grammar::Indefinite)) article = eat();
            room = recWords();
            
            std::cout << room << " -> " << loc << "\n";
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
        std::vector<std::string> verbs;
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
        
        if(have("you")) {
            eat();
            match("can");
            
            verbs.push_back(recWords(Grammar::Objective));
            match(Grammar::Objective);
            
            while(have(Token::Comma) || have("and")) {
                eat();
                verbs.push_back(recWords(Grammar::Objective));
                match(Grammar::Objective);
            }
            match(Token::Period);
        }
        
        if(have(Grammar::Possessive)) {
            eat();
            match("description");
            matchBeing();
            details = text();
            match(Token::QuotedString, "detailed descriptions must be between quotes");
        }

        
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
    
    std::string StoryParser::recWords(Grammar::Class stop) {
        std::string str = text();
        match(Token::Word);
        while(have(Token::Word) && !haveBeing() && !have(stop)) str += " " + eat();
        return str;
    }
}
