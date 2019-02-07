//===--------------------------------------------------------------------------------------------===
// story.hpp - Data structure representing a parsed story.
// This source is part of the Compass Engine
//
// Created on 2019-02-01 by Amy Parent <amy@amyparent.com>
// Copyright (c) 2019 Amy Parent
// Licensed under the MIT License
// =^•.•^=
//===--------------------------------------------------------------------------------------------===
#pragma once
#include <cstdint>
#include <map>
#include <set>
#include <string>
#include <utility>
#include <vector>

#define BYTECODE_MAX (128)

namespace Compass {
    
    class Story;
    class Place;
    class Thing;
    
    using StringID = std::size_t;
    
    // struct StringID {
    //     StringID() : id(0) {}
    //     StringID(std::size_t id) : id(id+1) {}
    //     operator bool() { return id == 0; }
    //     operator std::size_t() { return id-1; }
    //     std::size_t get() const { return id-1; }
    // private:
    //     std::size_t id;
    // };
    
    struct Link {
        StringID                target;
        std::string             direction;
    };
    
    struct Action {
        enum Kind { Native, Bytecode };
        
        Kind                    kind;
        StringID                verb = 0;
        std::uint32_t           bytecode[BYTECODE_MAX];
    };
    
    // MARK: - Entities
    
    struct Entity {
        std::vector<StringID>   things;
        
        StringID                id = 0;
        StringID                article = 0;
        StringID                name = 0;
        StringID                description = 0;
    };
    
    struct Place: public Entity {
        bool                    isVisited;
        std::vector<Link>       links;
    };
    
    struct Thing: public Entity {
        StringID                preposition= 0;
        StringID                location= 0;
        
        std::vector<StringID>   adjectives;
        std::vector<Action>     actions;
    };
    
    struct Context {
        StringID                            startID = 0;
        std::map<StringID, Place>           places;
        std::map<StringID, Thing>           things;
    };
    
    struct Verb {
        enum Kind {
            // Those are basic actions that any game gets by default. Dealt with by the engine.
            Go, Look, Take, Drop,
            // These are custom actions, defined by the story and compiled to bytecode
            StoryCode, CustomCode,
        };
        
        Kind        kind;
        StringID    verb;
        StringID    preposition;
    };
    
    // The story structure binds a whole thing together
    class Story {
    public:
        StringID uniqueID(const std::string& name) const;
        StringID uniqueID(StringID name) const;
        
        void addDirection(const std::string& dir);
        void addVerb(Verb::Kind kind, StringID verb, StringID preposition = 0);
        bool isVerb(const std::string& verb);
        const Verb& getVerb(const std::string& verb) const;
        
        // MARK: - Strings management
        StringID stringID(const std::string& str) const;
        const std::string& string(StringID id) const;
        
        Context                             prototype;
    private:
        friend class Semantics;
        
        // The string pool is mutable to allow const-qualified stringID
        mutable std::vector<std::string>    strings_;
        std::set<std::string>               directions_;
        std::map<std::string, Verb>         verbs_;
    };
}
