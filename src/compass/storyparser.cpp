//===--------------------------------------------------------------------------------------------===
// storyscriot.cpp - Implementation of the Compass Engine's story script file parser
// This source is part of the Compass Engine
//
// Created on 2019-01-30 by Amy Parent <amy@amyparent.com>
// Copyright (c) 2019 Amy Parent
// Licensed under the MIT License
// =^•.•^=
//===--------------------------------------------------------------------------------------------===
#include <iostream>
#include <vector>
#include <string>
#include <compass/storyparser.hpp>
#include <compass/story.hpp>
#include <compass/utils/string.hpp>

namespace Compass {
    
    StoryParser::StoryParser(const std::string& data, const Grammar& grammar)
        : RDParser(data, grammar) {}


    void StoryParser::error(const std::string& message) {
        std::cerr << "error: " << message << "\n";
        std::cerr << "token: " << text() << "(" << lexer.currentToken().type() << ")\n";
        abort();
    }
    
    Story StoryParser::compile() {
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
        sem_.resolve(story);
        return story;
    }
    
    void StoryParser::recDirectionDecl(Story& story) {
        std::string dir, opp;
        dir = recWords();
        matchBeing();
        match(Grammar::Indefinite, "directions must be *a* direction");
        match("direction", "I expected a direction");
        
        if(have(Token::Comma) || have("and")) {
            eat();
            match(Grammar::Definite, "expected a 'the' before opposite");
            match("opposite", "expected 'opposite'");
            matchBeing();
            opp = recWords();
        }
        match(Token::Period);
        sem_.addDirection(dir, opp);
        sem_.addDirection(opp, dir);
    }
    
    void StoryParser::recDecl(Story& story) {
        const auto name = recDeclStart(story);
        matchBeing("invalid verb after a thing or place");
        
        static bool foundFirst = false; // TODO: get rid of this ugly hack
        
        if(have(Grammar::Definite) || have(Grammar::Indefinite)) {
            eat();
            Place place;
            place.article = story.stringID(name.first);
            place.name = story.stringID(name.second);
            place.id = story.uniqueID(name.second);
            recRoomDecl(story, place);
        }
        else if(have(Grammar::Preposition)) {
            Thing thing;
            thing.article = story.stringID(name.first);
            thing.name = story.stringID(name.second);
            thing.id = story.uniqueID(name.second);
            recThingDecl(story, thing);
        }
        else {
            error("cannot infer what '" + name.second + "' is");
        }
    }
    
    StoryParser::Name StoryParser::recDeclStart(Story& story) {
        std::string article, name;
        
        if(have(Grammar::Definite) || have(Grammar::Indefinite)) article = String::toLower(eat());
        if(haveBeing()) error("things and places must have names");
        
        name = recWords();
        return std::make_pair(article, name);
    }
    
    void StoryParser::recRoomDecl(Story& story, Place& place) {
        if(have("room")) eat();
        else if(have("place")) eat();
        else error("this should be a place, but I can't see 'room' or 'place'");
        
        if(!have(Token::Period)) {
            recDirection(story, place);
            while(have(Token::Comma) || have("and")) {
                eat();
                recDirection(story, place);
            }
        }
        
        match(Token::Period);
        
        place.description = story.stringID(text());
        match(Token::QuotedString);
        sem_.addPlace(place);
    }
    
    void StoryParser::recDirection(Story& story, Place& place) {
            std::string dir = recWords(Grammar::Preposition);
            sem_.checkDirection(dir);
            match(Grammar::Preposition);
            std::string article, room;
            
            if(have(Grammar::Definite) || have(Grammar::Indefinite)) article = eat();
            room = recWords("and");
            sem_.markLink(story.uniqueID(room), story.uniqueID(place.name), dir);
    }
    
    void StoryParser::recThingDecl(Story& story, Thing& thing) {
        std::vector<std::string> verbs;
        
        // TODO: handle "compound" prepositions like "inside of"
        thing.preposition = story.stringID(text());
        match(Grammar::Preposition, "Things must be placed somewhere");
        // Parse the location of the object.
        if(have(Grammar::Definite) || have(Grammar::Indefinite)) eat();
        thing.location = story.uniqueID(recWords());
        
        
        // adjectives?
        while(have(Token::Comma) || have("and")) {
            eat();
            thing.adjectives.push_back(story.stringID(recWords("and")));
        }
        
        if(have(Token::Colon)) {
            eat();
            thing.description = story.stringID(text());
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
            thing.details = story.stringID(text());
            //details = text();
            match(Token::QuotedString, "detailed descriptions must be between quotes");
        }
        
        sem_.addThing(thing);
    }
    
    Action StoryParser::recAction(Story& story) {
        Action action;
        action.kind = Action::Native;
        action.verb = story.stringID(recWords(Grammar::Objective));
        match(Grammar::Objective);
        return action;
    }
}
