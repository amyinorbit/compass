//===--------------------------------------------------------------------------------------------===
// verb.hpp - Interface used to register, manipulate and recognise verbs
// This source is part of the Compass Engine
//
// Created on 2019-02-14 by Amy Parent <amy@amyparent.com>
// Copyright (c) 2019 Amy Parent
// Licensed under the MIT License
// =^•.•^=
//===--------------------------------------------------------------------------------------------===
#pragma once
#include <string>
#include <compass/utils/functional.hpp>

namespace Compass {
    
    struct Verb {
        enum Kind {
            // Those are basic actions that any game gets by default. Dealt with by the engine.
            Go, Look, Take, Drop, Inventory,
            // These are custom actions, defined by the story and compiled to bytecode
            StoryCode, CustomCode,
        };
        
        Kind        kind;
        std::string present, past, participle, infinitive;
    };
    
    class VerbBuilder {
    public:
        
        VerbBuilder(const std::string& present);
        
        VerbBuilder& past(const std::string& past);
        VerbBuilder& participle(const std::string& participle);
        VerbBuilder& infinitive(const std::string& infinitive);
        
        Verb make(Verb::Kind kind);
        
    private:
        std::string present_;
        optional<std::string> past_, participle_, infinitive_ = {};
    };
}
