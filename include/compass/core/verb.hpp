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
#include <cstdint>
#include <string>
#include <compass/core/functional.hpp>

namespace Compass {
    
    struct Verb {
        enum Kind: std::uint8_t  {
            Go          = 0x01,
            Look        = 0x02,
            Take        = 0x03,
            Drop        = 0x04,
            Inventory   = 0x05,
            StoryCode   = 0x06,
            CustomCode  = 0x07,
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
