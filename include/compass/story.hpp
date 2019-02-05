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

    struct Link {
        StringID                target;
        std::string             direction;
    };
    
    struct Action {
        enum Kind { Native, Bytecode };
        
        Kind                    kind;
        StringID                verb;
        std::uint32_t           bytecode[BYTECODE_MAX];
    };
    
    // MARK: - Entities

    struct Entity {
        std::vector<StringID>   things;
        
        StringID                id;
        StringID                article;
        StringID                name;
        StringID                description;
    };
    
    struct Place: public Entity {
        bool                    isVisited;
        std::vector<Link>       links;
    };
    
    struct Thing: public Entity {
        StringID                preposition;
        StringID                location;
        
        std::vector<StringID>   adjectives;
        std::vector<Action>     actions;
    };
    
    class Context {
    public:
        
        const Place& place(StringID uniqueID) const;
        Place& place(StringID uniqueID);
        
        const Thing& thing(StringID uniqueID) const;
        Thing& thing(StringID uniqueID);
        
        Place& start() { return places_[start_]; }
        const Place& start() const { return places_.at(start_); }
        
    private:
        friend class Semantics;
        
        StringID                            start_;
        std::map<StringID, Place>           places_;
        std::map<StringID, Thing>           things_;
    };
    
    // The story structure binds a whole thing together
    class Story {
    public:
        StringID uniqueID(const std::string& name) const;
        StringID uniqueID(StringID name) const;
        
        void addDirection(const std::string& dir);
        
        // MARK: - Strings management
        StringID stringID(const std::string& str) const;
        const std::string& string(StringID id) const;
        

        Context                             prototype;
    private:
        friend class Semantics;
        
        // The string pool is mutable to allow const-qualified stringID
        mutable std::vector<std::string>    strings_;
        std::set<std::string>               directions_;
    };
    
}
