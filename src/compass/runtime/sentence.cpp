//===--------------------------------------------------------------------------------------------===
// sentence.cpp - Implementation of the Compass command parser
// This source is part of the Compass Engine
//
// Created on 2019-02-05 by Amy Parent <amy@amyparent.com>
// Copyright (c) 2019 Amy Parent
// Licensed under the MIT License
// =^•.•^=
//===--------------------------------------------------------------------------------------------===
#include <iostream>
#include <compass/runtime/sentence.hpp>

namespace Compass {
    using namespace Compass::Language;
    
    Sentence::Sentence(const Story& story, const std::string& data, const Grammar& grammar)
        : story_(story), grammar_(grammar), RDParser(data, *this) {}
    
    result<Sentence::Command> Sentence::parse() {
        Command cmd;
        
        eat();
        cmd.verb = text();
        expect(Token::Word);
        
        if(have(Grammar::Preposition)) cmd.preposition = eat();
        if(have(Grammar::Definite) || have(Grammar::Indefinite)) eat();
        
        if(have(Token::Word)) {
            cmd.object = recWords();
        }
        if(error_) return make_unexpected(*error_);
        return cmd;
    }
    
    void Sentence::error(const std::string& message) {
        error_.emplace(message);
    }
}
