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
#include <compass/verb.hpp>
#include <compass/utils/functional.hpp>

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
    
    struct Relation {
        enum Kind { In, On, Under };
        
        Kind            kind;
        std::string     id;
    };
    
    struct Action {
        enum Kind { Native, Bytecode };
        
        Kind                    kind;
        std::string             verb;
        std::uint32_t           bytecode[BYTECODE_MAX];
    };
    
    // MARK: - Entities
    
    struct Entity {
        enum Kind { Place, Thing, Unknown };
        
        using RelationList = std::map<std::string, Relation::Kind>;
        
        Kind                    kind            = Unknown;
        std::string             id              = "";
        
        bool                    isSeen          = false;
        StringID                article         = 0;
        StringID                name            = 0;
        StringID                description     = 0;
        
        optional<Relation>      container       = {};
        
        RelationList            things;
        std::vector<Action>     actions;
        std::vector<Link>       links;
    };
    
    struct Context {
        std::string                         startID;
        std::map<std::string, Entity>       entities;
    };
    
    // The story structure binds a whole thing together
    class Story {
    public:
        Story();
        
        std::string uniqueID(const std::string& name) const;
        std::string uniqueID(StringID name) const;
        
        void addDirection(const std::string& dir);
        
        void addVerb(const std::string& present, Verb::Kind kind);
        optional<Verb::Kind> verb(const std::string& verb) const;
        
        // MARK: - Strings management
        StringID intern(const std::string& str) const;
        const std::string& string(StringID id) const;
        
        Context                             prototype;
        optional<std::string>               author;
        optional<std::string>               title;
    private:
        friend class Semantics;
        
        // The string pool is mutable to allow const-qualified intern
        mutable std::vector<std::string>    strings_;
        std::set<std::string>               directions_;
        std::map<std::string, Verb::Kind>   verbs_;
    };
}
