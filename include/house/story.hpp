//===--------------------------------------------------------------------------------------------===
// story.hpp - Data structure representing a parsed story.
// This source is part of the House Engine
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

namespace House {

    class Story;
    class Place;
    class Thing;
    //struct Direction;
    
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
        
        StringID                uniqueID;
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
    
    // The story structure binds a whole thing together
    class Story {
    public:
        StringID uniqueID(const std::string& name);
        StringID uniqueID(StringID name);
        StringID addPlace(Place place);
        StringID addThing(StringID entity, Thing thing);
        
        Place& place(StringID uniqueID);
        Place& place(const std::string& uniqueID);
        Thing& thing(StringID uniqueID);
        Thing& thing(const std::string& uniqueID);
        
        void addDirection(const std::string& dir);
        void addLink(StringID from, StringID to, const std::string& direction);
        
        // MARK: - Strings management
        StringID installString(const std::string& str);
        const std::string& string(StringID id) const;
        
    private:
        friend class Semantics;
        std::pair<StringID, std::string> makeID(StringID name);
        
        std::vector<std::string>            strings_;
        std::set<std::string>               directions_;
        std::map<std::string, Place>        places_;
        std::map<std::string, Thing>        things_;
    };
    
}
