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
#include <compass/utils/maybe.hpp>

#define BYTECODE_MAX (128)

namespace Compass {
    
    class Story;
    class Place;
    class Thing;
    
    using StringID = std::size_t;
    
    struct Link {
        std::string             target;
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
        std::set<std::string>   things;
        
        std::string             id;
        StringID                article = 0;
        StringID                name = 0;
        StringID                description = 0;
    };
    
    enum class Lock {
        None, Key, Password
    };
    
    struct Place: public Entity {
        Lock                    lock = Lock::None;
        Maybe<std::string>      lockID;
        bool                    isVisited = false;
        std::vector<Link>       links;
    };
    
    struct Thing: public Entity {
        Maybe<std::string>      location;
        StringID                preposition= 0;
        StringID                details = 0;
        
        std::vector<StringID>   adjectives;
        std::vector<Action>     actions;
    };
    
    struct Context {
        std::string                         startID;
        std::map<std::string, Place>        places;
        std::map<std::string, Thing>        things;
    };
    
    struct Verb {
        enum Kind {
            // Those are basic actions that any game gets by default. Dealt with by the engine.
            Go, Look, Take, Drop, Inventory,
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
        Story();
        
        std::string uniqueID(const std::string& name) const;
        std::string uniqueID(StringID name) const;
        
        void addDirection(const std::string& dir);
        void addVerb(Verb::Kind kind, StringID verb, StringID preposition = 0);
        bool isVerb(const std::string& verb) const;
        const Verb& getVerb(const std::string& verb) const;
        
        // MARK: - Strings management
        StringID stringID(const std::string& str) const;
        const std::string& string(StringID id) const;
        
        Context                             prototype;
        Maybe<std::string>                  author;
        Maybe<std::string>                  title;
    private:
        friend class Semantics;
        
        // The string pool is mutable to allow const-qualified stringID
        mutable std::vector<std::string>    strings_;
        std::set<std::string>               directions_;
        std::map<std::string, Verb>         verbs_;
    };
}
