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
#include <house/storyparser.hpp>
#include <house/story.hpp>

namespace House {
    
    void StoryParser::compile() {
        eat();
        
        Story story;
        
        if(have("directions")) {
            eat();
            match(Token::Colon);
            
            while(!have("story")) {
                recDirectionDecl(story);
            }
        }
        match("story");
        match(Token::Colon);
        
        while(!have(Token::End)) {
            recDecl(story);
        }
        match(Token::End);
        
    }
    
    void StoryParser::recDirectionDecl(Story& story) {
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
        story.installDirection(dir);
        std::cout << dir.direction << " <-> " << dir.opposite << "\n";
    }
    
    void StoryParser::recDecl(Story& story) {
        const auto name = recDeclStart(story);
        matchBeing("invalid verb after a thing or place");
        
        if(have(Grammar::Definite) || have(Grammar::Indefinite)) {
            eat();
            Place place;
            place.article = story.installString(name.first);
            place.name = story.installString(name.second);
            recRoomDecl(story, place);
        }
        else if(have(Grammar::Preposition)) {
            Thing thing;
            thing.article = story.installString(name.first);
            thing.name = story.installString(name.second);
            recThingDecl(story, thing);
        }
        else {
            syntaxError("cannot infer what '" + name.second + "' is");
        }
    }
    
    StoryParser::Name StoryParser::recDeclStart(Story& story) {
        std::string article, name;
        
        if(have(Grammar::Definite) || have(Grammar::Indefinite)) article = eat();
        if(haveBeing()) syntaxError("things and places must have names");
        
        name = recWords();
        return std::make_pair(article, name);
    }
    
    void StoryParser::recRoomDecl(Story& story, Place& place) {
        if(have("room")) eat();
        else if(have("place")) eat();
        else syntaxError("this should be a place, but I can't see 'room' or 'place'");
        
        if(!have(Token::Period)) {
            // TODO: handle directions better. Not quite sure how yet.
            std::string dir = recWords(Grammar::Preposition);
            //if(!story.directionIsValid(dir))
            match(Grammar::Preposition);
            std::string article, room;
            
            if(have(Grammar::Definite) || have(Grammar::Indefinite)) article = eat();
            room = recWords();
            
            std::cout << room << " -> " << dir << "\n";
        }
        
        match(Token::Period);
        
        place.description = story.installString(text());
        match(Token::QuotedString);
    }
    
    void StoryParser::recThingDecl(Story& story, Thing& thing) {
        std::vector<std::string> verbs;
        
        thing.preposition = story.installString(text());
        match(Grammar::Preposition, "Things must be placed somehwere");
        // Parse the location of the object.
        if(have(Grammar::Definite) || have(Grammar::Indefinite)) eat();
        thing.location = story.uniqueID(recWords());
        
        
        // adjectives?
        while(have(Token::Comma) || have("and")) {
            eat();
            thing.adjectives.push_back(story.installString(recWords("and")));
        }
        
        if(have(Token::Colon)) {
            eat();
            thing.description = story.installString(text());
            match(Token::QuotedString, "descriptions of short things should be in quotes");
        }
        match(Token::Period);
        
        if(have("you")) {
            eat();
            match("can");
            thing.actions.push_back(recAction(story));
            
            while(have(Token::Comma) || have("and")) {
                eat();
                thing.actions.push_back(recAction(story));
            }
            match(Token::Period);
        }
        
        // TODO: handle detailed descriptions
        if(have(Grammar::Possessive)) {
            eat();
            match("description");
            matchBeing();
            //details = text();
            match(Token::QuotedString, "detailed descriptions must be between quotes");
        }
    }
    
    Action StoryParser::recAction(Story& story) {
        Action action;
        action.kind = Action::Native;
        action.verb = story.installString(recWords(Grammar::Objective));
        match(Grammar::Objective);
        return action;
    }
    
    // std::string StoryParser::recPosition() {
    //     std::string relative = text();
    //     match(Token::Word);
    //     if(have(Grammar::Preposition)) eat();
    //
    //     if(have(Grammar::Definite)) eat();
    //     std::string name = text();
    //     match(Token::Word);
    //     while(have(Token::Word)) {
    //         name += " " + eat();
    //     }
    //     return relative + ", " + name;
    // }
    
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