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

namespace Compass {
    using namespace std::placeholders;
    
    void Parser::error(const std::string& e) {
        std::cerr << "[PARSER]: " << e << std::endl;
        abort();
        error_.emplace(e);
        fail();
    }
    
    result<Story> Parser::compile() {
        eat();
        
        if(match("directions")) {
            expect(Token::Colon);
            while(!have("story")) {
                recDirectionDecl();
            }
        }
        
        expect("story");
        expect(Token::Colon);
        
        while(!have(Token::End) && !isFailed()) {
            if(have("there"))
                recThereDecl();
            else if(have(Token::QuotedString))
                recDescription();
            else
                recBeDecl();
            expect(Token::Period);
        }
        
        return make_unexpected(*error_);
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
    
    // "there", present-being, subject, [location-spec] ;
    void Parser::recThereDecl() {
        expect("there");
        expectBeing();
        auto noun = recNoun();
        
        sema_.declare(noun);
        
        std::cout << noun.text << "\n";
        
        if(have("in") || have("on")) {
            auto preposition = eat();
            auto container = recNoun();
            std::cout << " - " << preposition << " " << container.text << "\n";
        }
    }

    void Parser::recBeDecl() {
        auto name = recSubject();
        optional<Noun> what = {};
        expectBeing();
        
        // TODO: change to recObject();
        if(!haveDirection() && !have("in") && !have("on")) {
            what = recObject();
            std::cout << " [" << what->text << "]\n";
        }
        
        //sema_.declare(name, what);
        
        // Parse location. If we get there, we change from specifying a property to declaring
        // a new thing or room.
        if(have("in") || have("on")) {
            recContainer();
        }
        else if(haveDirection()) {
            recDirectionList();
        }
    }
    
    optional<Noun> Parser::recSubject() {
        if(match(Grammar::Subjective)) {
            std::cout << "subjective\n";
            return {};
        } else {
            auto noun = recNoun();
            std::cout << noun.text << "\n";
            return noun;
        }
    }
    
    Noun Parser::recObject() {
        
        Noun noun{{}, ""};
        if(have(Grammar::Definite) | have(Grammar::Indefinite)) {
            noun.article = eat();
        }
        
        noun.text = text();
        expect(Token::Word);
        
        while(have(Token::Word)&& !have(Grammar::Preposition) && !haveDirection()) {
            noun.text += " " + eat();
        }
        return noun;
    }
    
    void Parser::recDirectionList() {
        recDirection();
        while(match(Token::Comma) || match("and")) {
            recDirection();
        }
    }

    void Parser::recDirection() {
        std::string dir = text();
        expect(Token::Word);
        
        std::string preposition = text();
        expect(Token::Word);
        
        Noun place = recNoun();
        
        std::cout << "  -> " << dir << " " << preposition << " " << place.text << "\n";
    }

    void Parser::recContainer() {
        if(!have("in") && !have("on")) {
            error("Things can be in or on other things only");
            return;
        }
        auto preposition = eat();
        auto container = recNoun();
        sema_.setContainer({}, container.text);
        std::cout << " - " << preposition << " " << container.text << "\n";
    }
    
    void Parser::recDescription() {
        auto look = text();
        expect(Token::QuotedString);
        sema_.setDescription(look);
        std::cout << "desc: '" << look << "'\n";
    }
    
    void Parser::recPassiveAbility() {
        // TODO: implementation
    }
    
    void Parser::recActiveAbility() {
        // TODO: implementation
    }
    
    Noun Parser::recNoun() {
        Noun noun{{}, ""};
        if(have(Grammar::Definite) | have(Grammar::Indefinite)) {
            noun.article = eat();
        }
        noun.text = text();
        expect(Token::Word);
        
        while(have(Token::Word) && !haveBeing() && !have(Grammar::Preposition)) {
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
