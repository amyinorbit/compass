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
    
    void FParser::compile() {
        eat();
        while(!have(Token::End)) {
            recEntity();
        }
    }
    
    result<FEntity> FParser::recEntity() {
        return recDeclNoun()
            .and_then(std::bind(&FParser::recEntityKind, this, _1))
            .and_then(std::bind(&FParser::recDeclLocations, this, _1));
    }
    
    result<FParser::Noun> FParser::recDeclNoun() {
        if(have("there"))
            return recThereDecl();
        if(have(Grammar::Indefinite) || have(Grammar::Definite)) 
            return recBeDecl();
        return make_unexpected("I don't understand what this sentence is");
    }
    
    result<FParser::Noun> FParser::recThereDecl() {
        if(!match("there")) return make_unexpected("A thing should be described by 'there is...'");
        if(!matchBeing()) return make_unexpected("A thing must *be*");
        
        Noun noun{{}, ""};
        if(have(Grammar::Definite) || have(Grammar::Indefinite))
            noun.article.emplace(eat());
        noun.name = recWords();
        return noun;
    }
    
    result<FParser::Noun> FParser::recBeDecl() {
        Noun noun{{}, ""};
        if(have(Grammar::Definite) || have(Grammar::Indefinite))
            noun.article.emplace(eat());
        noun.name = recWords();
        if(!matchBeing()) return make_unexpected("A thing must *be*");
        return noun;
    }
    
    result<FEntity> FParser::recEntityKind(Noun noun) {
        FEntity entity;
        //entity.article
        std::cout << "created entity: " << noun.name << "\n";
        match(Grammar::Indefinite);
        if(match("room") || match("place")) {
            std::cout << "it's a place\n";
            entity.kind = FEntity::Place;
            return entity;
        }
        if(match("thing")) {
            std::cout << "it's a thing\n";
            entity.kind = FEntity::Thing;
            return entity;
        }
        return make_unexpected("I'm not sure what '" + noun.name + "' is");
    }
    
    result<FEntity> FParser::recDeclLocations(FEntity entity) {
        if(!match(Token::Period)) return make_unexpected("Sentences must end with a colon");
        return entity;
    }
}
