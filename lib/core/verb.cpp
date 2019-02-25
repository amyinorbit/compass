//===--------------------------------------------------------------------------------------------===
// verb.cpp - Implementation of basic verb handling
// This source is part of the Compass Engine
//
// Created on 2019-02-14 by Amy Parent <amy@amyparent.com>
// Copyright (c) 2019 Amy Parent
// Licensed under the MIT License
// =^•.•^=
//===--------------------------------------------------------------------------------------------===
#include <compass/core/verb.hpp>

namespace Compass {
    
    VerbBuilder::VerbBuilder(const std::string& present)
        : present_(present) {}
    
    VerbBuilder& VerbBuilder::past(const std::string& past) {
        past_ = past;
        return *this;
    }
    
    VerbBuilder& VerbBuilder::participle(const std::string& participle) {
        participle_ = participle;
        return *this;
    }
    
    VerbBuilder& VerbBuilder::infinitive(const std::string& infinitive) {
        infinitive_ = infinitive;
        return *this;
    }
    
    std::string pastOf(const std::string& root) {
        if(!root.size()) return "";
        if(root.back() == 'e')
            return root + "d";
        return root + "ed";
    }
    
    Verb VerbBuilder::make(Verb::Kind kind) {
        Verb verb;
        verb.kind = kind;
        verb.present = present_;
        
        verb.past = *past_.disjunction(pastOf(present_));
        verb.participle = *participle_.disjunction(pastOf(present_));
        verb.infinitive = *infinitive_.disjunction(present_ + "ing");
        
        return verb;
    }
}
