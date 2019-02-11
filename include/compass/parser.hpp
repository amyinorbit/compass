//===--------------------------------------------------------------------------------------------===
// parser.hpp - Compass story functional-style parser
// This source is part of the Compass Engine
//
// Created on 2019-02-10 by Amy Parent <amy@amyparent.com>
// Copyright (c) 2019 Amy Parent
// Licensed under the MIT License
// =^•.•^=
//===--------------------------------------------------------------------------------------------===
#pragma once

#include <string>
#include <compass/grammar.hpp>
#include <compass/rdparser.hpp>
#include <compass/story.hpp>
#include <compass/utils/functional.hpp>

namespace Compass {
    
    // TODO: merge that into the main story system
    struct FEntity {
        enum Kind { Thing, Place, Custom };
        
        Kind        kind;
        std::string uniqueID;
        
        StringID    article         = 0;
        StringID    name            = 0;
        
        StringID    look            = 0;
        StringID    description     = 0;    
    };
    
    class FParser: public RDParser {
    public:
        FParser(const std::string& source, const Grammar& grammar) : RDParser(source, grammar) {}
        /*result<Story>*/ void compile();
        
    private:
        void error(const std::string& e) {}
        
        struct Noun {
            optional<std::string> article;
            std::string name;
        };
        
        result<FEntity> recEntity();
        result<Noun> recDeclNoun();
        
        result<Noun> recThereDecl();
        result<Noun> recBeDecl();
        
        result<FEntity> recEntityKind(Noun noun);
        result<FEntity> recDeclLocations(FEntity entity);
        
        result<Noun> recNoun();
        
        
        BasicEnglish grammar_;
    };
    
}
