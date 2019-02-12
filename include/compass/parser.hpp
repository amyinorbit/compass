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
#include <set>
#include <string>
#include <compass/grammar.hpp>
#include <compass/rdparser.hpp>
#include <compass/story.hpp>
#include <compass/contextsema.hpp>
#include <compass/utils/functional.hpp>

namespace Compass {
    
    // TODO: merge that into the main story system
    
    class Parser: public RDParser {
    public:
        Parser(const std::string& source, const Grammar& grammar) : RDParser(source, grammar) {}
        result<Story> compile();
        
    private:
        void error(const std::string& e);
        
        void recDirectionDecl();
        
        void recThereDecl();
        void recBeDecl();
        
        optional<Noun> recSubject();
        Noun recObject();
        
        void recDirectionList();
        void recDirection();
        void recContainer();
        
        void recDescription();
        
        void recPassiveAbility();
        void recActiveAbility();
        Noun recNoun();
        
        bool haveDirection() const;
        void declareDirection(const std::string& direction, const optional<std::string>& opposite);
        
        std::set<std::string>   directions_;
        optional<std::string>   error_;
        
        ContextSema             sema_;
    };
    
}
