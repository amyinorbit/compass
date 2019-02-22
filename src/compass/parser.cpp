//===--------------------------------------------------------------------------------------------===
// parser.cpp - Implementation of the compass story format functional parser.
// This source is part of the Compass Engine
//
// Created on 2019-02-10 by Amy Parent <amy@amyparent.com>
// Copyright (c) 2019 Amy Parent
// Licensed under the MIT License
// =^•.•^=
//===--------------------------------------------------------------------------------------------===
#include <cassert>
#include <iostream>
#include <compass/compiler.hpp>
#include <compass/parser.hpp>
#include <compass/sema.hpp>
#include <compass/utils/string.hpp>

namespace Compass {
    using namespace std::placeholders;
    
    void Parser::run() {
        eat();
        
        if(have(Token::QuotedString)) {
            recTitleAndAuthor();
        }
        
        while(!have(Token::End) && !driver.isFailed()) {
            if(have("there")) {
                recThereSentence();
                expect(Token::Period);
            }
            else if(have(Token::QuotedString)) {
                recDescription();
                match(Token::Period);
            }
            else if(have("use")) {
                recImport();
                expect(Token::Period);
            }
            else {
                recActiveSentence();
                expect(Token::Period);
            }
        }
    }
    
    void Parser::recTitleAndAuthor() {
        auto title = text();
        expect(Token::QuotedString);
        expect("by");
        auto author = text();
        expect(Token::Word);
        while(have(Token::Word))
            author += " " + eat();
        
        expect(Token::Period);
        
        compiler().sema().setTitle(title);
        compiler().sema().setAuthor(author);
    }
    
    void Parser::recImport() {
        expect("use");
        auto name = recWords();
        compiler().include(name);
    }
    
    // MARK: - Story Parsing system
    // MARK: - There-decl
    
    // there-sentence  = "there", present-being, noun, container-rel ;
    void Parser::recThereSentence() {
        expect("there");
        expectBeing();
        auto thing = recNoun();
        compiler().sema().declare(Entity::Thing, thing);
        recRelRelation();
    }
    
    // MARK: - active sentence parsing
    
    // active-sentence = subject, (be-sentence | can-sentence);
    void Parser::recActiveSentence() {
        auto subject = recSubject();
        if(haveBeing())
            recBeSentence(subject);
        else if(have("can"))
            recCanSentence(subject);
        else
            driver.error("<must have either 'can' or 'be'>");
    }
    
    // be-sentence     = present-being, ( property | be-spec ) ;
    void Parser::recBeSentence(const optional<Noun>& subject) {
        expectBeing();
        if(have("in") || have("on") || haveDirection() || have(Grammar::Indefinite))
            recBeDecl(subject);
        else
            recPropertyDecl(subject);
    }
    
    void Parser::recCanSentence(const optional<Noun>& subject) {
        expect("can");
        expect("be");
        const auto name = subject.map([](const auto& n) { return n.text; });
        
        auto participle = text();
        expect(Token::Word);
        compiler().sema().addVerb(name, participle);
        
        while(match(Token::Comma) || match("and")) {
            participle = text();
            expect(Token::Word);
            compiler().sema().addVerb(name, participle);
        }
    }
    
    // be-spec         = [directions | container-rel];
    void Parser::recBeDecl(const optional<Noun>& subject) {
        bool declared = false;
        if(match(Grammar::Indefinite)) {
            if(!subject) {
                driver.error("You can only declare places or things with names");
                return;
            }

            // TODO: find a better way to test this, this is a bit ugly
            if(match("direction")) {
                recDirectionDecl(subject->text);
                return;
            } else {
                auto kind = recClass();
                compiler().sema().declare(kind, *subject);
                declared = true;
            }
        }
        
        if(have("in") || have("on") || have("under")) {
            if(!declared && subject) compiler().sema().declare(Entity::Thing, *subject, true);
            recRelRelation();
        } else if(haveDirection()) {
            if(!declared && subject) compiler().sema().declare(Entity::Place, *subject, true);
            recRelDirection();
            while(match(Token::Comma) || match("and"))
                recRelDirection();
        }
    }
    
    void Parser::recPropertyDecl(const optional<Noun>& subject) {
        std::string prop = text();
        expect(Token::Word);
        while(have(Token::Word))
            prop += " " + eat();
    }
    
    void Parser::recDirectionDecl(const std::string& dir) {
        optional<std::string> opp;
        
        if(have(Token::Comma) || have("and")) {
            eat();
            expect(Grammar::Definite, "expected a 'the' before opposite");
            expect("opposite", "expected 'opposite'");
            expectBeing();
            opp = recWords();
        }
        compiler().sema().declareDirection(dir, opp);
    }
    
    void Parser::recRelDirection() {
        auto direction = text(); // TODO: probably needs to be more strict
        expect(Token::Word);
        expect(Grammar::Preposition);
        auto place = recNoun();
        compiler().sema().addLink({}, place.text, direction);
    }
    
    void Parser::recRelRelation() {
        auto kind = recRelationLoc();
        auto place = recNoun();
        compiler().sema().setRelation({}, kind, place.text);
    }
    
    Relation::Kind Parser::recRelationLoc() {
        if(match("in"))
            return Relation::In;
        if(match("on"))
            return Relation::On;
        if(match("under"))
            return Relation::Under;
        assert("unreachable");
    }
    
    optional<Noun> Parser::recSubject() {
        if(match(Grammar::Subjective))
            return {};
        return recNoun();
    }
    
    void Parser::recDescription() {
        auto description = text();
        expect(Token::QuotedString);
        compiler().sema().setDescription(description);
    }
    
    Entity::Kind Parser::recClass() {
        if(match("place") || match("room"))
            return Entity::Place;
        eat();
        return Entity::Thing;
    }
    
    Noun Parser::recNoun() {
        Noun noun{{}, ""};
        if(have(Grammar::Definite) | have(Grammar::Indefinite)) {
            noun.article = String::toLower(eat());
        }
        noun.text = text();
        expect(Token::Word);
        
        while(have(Token::Word) && !haveBeing() && !have(Grammar::Preposition) && !have("and")) {
            noun.text += " " + eat();
        }
        return noun;
    }
    
    bool Parser::haveDirection() const {
        return compiler().sema().isDirection(text());
    }
}
