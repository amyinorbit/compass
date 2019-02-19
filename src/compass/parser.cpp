//===--------------------------------------------------------------------------------------------===
// parser.cpp - Implementation of the compass story format functional parser.
// This source is part of the Compass Engine
//
// Created on 2019-02-10 by Amy Parent <amy@amyparent.com>
// Copyright (c) 2019 Amy Parent
// Licensed under the MIT License
// =^•.•^=
//===--------------------------------------------------------------------------------------------===
#include <iostream>
#include <compass/parser.hpp>
#include <compass/utils/string.hpp>

namespace Compass {
    using namespace std::placeholders;
    
    void Parser::error(const std::string& e) {
        std::cerr << "[PARSER/" << lexer.currentToken().type() << "]: " << e << std::endl;
        //abort();
        error_.emplace(e);
        fail();
    }
    
    result<Story> Parser::compile() {
        eat();
        
        if(have(Token::QuotedString)) {
            recTitleAndAuthor();
        }
        
        if(match("directions")) {
            expect(Token::Colon);
            while(!have("story")) {
                recDirectionDecl();
            }
        }
        
        expect("story");
        expect(Token::Colon);
        
        while(!have(Token::End) && !isFailed()) {
            if(have("there")) {
                recThereSentence();
                expect(Token::Period);
            }
            else if(have(Token::QuotedString)) {
                recDescription();
                match(Token::Period);
            }
            else {
                recActiveSentence();
                expect(Token::Period);
            }
        }
        return sema_.resolve();
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
        
        sema_.setTitle(title);
        sema_.setAuthor(author);
    }
    
    void Parser::recDirectionDecl() {
        std::string dir;
        optional<std::string> opp;
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
        declareDirection(dir, opp);
        
        sema_.declareDirection(dir, opp);
    }
    
    // MARK: - Story Parsing system
    // MARK: - There-decl
    
    // there-sentence  = "there", present-being, noun, container-rel ;
    void Parser::recThereSentence() {
        expect("there");
        expectBeing();
        auto thing = recNoun();
        sema_.declare(Entity::Thing, thing);
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
            error("<must have either 'can' or 'be'>");
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
        sema_.addVerb(name, participle);
        //std::cout << " *~" << participle << "\n";
        while(match(Token::Comma) || match("and")) {
            participle = text();
            expect(Token::Word);
            sema_.addVerb(name, participle);
        }
    }
    
    // be-spec         = [directions | container-rel];
    void Parser::recBeDecl(const optional<Noun>& subject) {
        bool declared = false;
        if(have(Grammar::Indefinite)) {
            if(!subject) {
                error("You can only declare places or things with names");
                return;
            }
            auto kind = recClass();
            sema_.declare(kind, *subject);
            declared = true;
        }
        
        if(have("in") || have("on") || have("under")) {
            if(!declared && subject) sema_.declare(Entity::Thing, *subject, true);
            recRelRelation();
        } else if(haveDirection()) {
            if(!declared && subject) sema_.declare(Entity::Place, *subject, true);
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
    
    void Parser::recRelDirection() {
        auto direction = text(); // TODO: probably needs to be more strict
        expect(Token::Word);
        expect(Grammar::Preposition);
        auto place = recNoun();
        sema_.addLink({}, place.text, direction);
    }
    
    void Parser::recRelRelation() {
        auto kind = recRelationLoc();
        auto place = recNoun();
        sema_.setRelation({}, kind, place.text);
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
        sema_.setDescription(description);
    }
    
    Entity::Kind Parser::recClass() {
        expect(Grammar::Indefinite, "Must have 'a' thing or 'a' place");
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
        return directions_.find(lexer.currentToken().text) != directions_.end();
    }
    
    void Parser::declareDirection(const std::string& direction, const optional<std::string>& opposite) {
        directions_.insert(direction);
        if(opposite)
            directions_.insert(*opposite);
    }
}
