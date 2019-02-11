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
        
        if(have(Token::QuotedString)) {
            recTitleDecl(story);
        }
        
        if(match("directions")) {
            expect(Token::Colon);
            
            while(!have("story")) {
                recDirectionDecl(story);
            }
        }
        
        expect("story");
        expect(Token::Colon);
        
        while(!have(Token::End)) {
            recDecl(story);
        }
        expect(Token::End);
        sem_.resolve(story);
        return story;
    }
    
    void StoryParser::recTitleDecl(Story& story) {
        story.title = text();
        expect(Token::QuotedString);
        if(have(Token::Comma)) eat();
        if(have("by")) {
            eat();
            story.author = recWords();
            expect(Token::Period);
        }
    }
    
    void StoryParser::recDirectionDecl(Story& story) {
        std::string dir, opp;
        dir = recWords();
        expectBeing();
        expect(Grammar::Indefinite, "directions must be *a* direction");
        expect("direction", "I expected a direction");
        
        if(have(Token::Comma) || have("and")) {
            eat();
            expect(Grammar::Definite, "expected a 'the' before opposite");
            expect("opposite", "expected 'opposite'");
            expectBeing();
            opp = recWords();
        }
        expect(Token::Period);
        sem_.addDirection(dir, opp);
        sem_.addDirection(opp, dir);
    }
    
    void StoryParser::recDecl(Story& story) {
        const auto name = recDeclStart(story);
        expectBeing("invalid verb after a thing or place");
        
        static bool foundFirst = false; // TODO: get rid of this ugly hack
        
        if(match(Grammar::Definite) || match(Grammar::Indefinite)) {
            
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
        if(!match("room") && !match("place"))
            error("this should be a place, but I can't see 'room' or 'place'");
        
        if(!have(Token::Period)) {
            recDirection(story, place);
            while(match(Token::Comma) || match("and")) {
                recDirection(story, place);
            }
        }
        
        expect(Token::Period);
        place.description = story.stringID(text());
        expect(Token::QuotedString);
        
        match(Token::Period);
        sem_.addPlace(place);
    }
    
    void StoryParser::recDirection(Story& story, Place& place) {
            std::string dir = recWords(Grammar::Preposition);
            sem_.checkDirection(dir);
            expect(Grammar::Preposition);
            std::string article, room;
            
            if(have(Grammar::Definite) || have(Grammar::Indefinite)) article = eat();
            room = recWords("and");
            sem_.markLink(story.uniqueID(room), story.uniqueID(place.name), dir);
    }
    
    void StoryParser::recThingDecl(Story& story, Thing& thing) {
        std::vector<std::string> verbs;
        
        // TODO: handle "compound" prepositions like "inside of"
        thing.preposition = story.stringID(text());
        expect(Grammar::Preposition, "Things must be placed somewhere");
        // Parse the location of the object.
        if(have(Grammar::Definite) || have(Grammar::Indefinite)) eat();
        thing.location = story.uniqueID(recWords());
        
        
        // adjectives?
        while(match(Token::Comma) || match("and")) {
            thing.adjectives.push_back(story.stringID(recWords("and")));
        }
        
        if(match(Token::Colon)) {
            thing.description = story.stringID(text());
            expect(Token::QuotedString, "descriptions of short things should be in quotes");
        }
        expect(Token::Period);
        
        if(match("you")) {
            expect("can");
            thing.actions.push_back(recAction(story));
            
            while(match(Token::Comma) || match("and"))
                thing.actions.push_back(recAction(story));
            
            expect(Token::Period);
        }
        
        if(match(Grammar::Possessive)) {
            
            expect("description");
            expectBeing();
            thing.details = story.stringID(text());
            expect(Token::QuotedString, "detailed descriptions must be between quotes");
        }
        
        match(Token::Period);
        sem_.addThing(thing);
    }
    
    Action StoryParser::recAction(Story& story) {
        Action action;
        action.kind = Action::Native;
        action.verb = story.stringID(recWords(Grammar::Objective));
        expect(Grammar::Objective);
        return action;
    }
}
