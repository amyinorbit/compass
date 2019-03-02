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
#include <compass/core/functional.hpp>
#include <compass/core/verb.hpp>

#define BYTECODE_MAX (128)

namespace Compass {
    
    namespace Compiler {
        class Sema;
    }
    
    class Story;
    class Place;
    class Thing;
    
    using StringID = std::size_t;
    
    struct Link {
        std::string             target;
        std::string             direction;
    };
    
    struct Relation {
        enum Kind: std::uint8_t {
            In      = 0x01,
            On      = 0x02,
            Under   = 0x03
        };
        
        Kind            kind;
        std::string     id;
    };
    
    struct Action {
        enum Kind: std::uint8_t {
            Native      = 0x01,
            Bytecode    = 0x02
        };
        
        Kind                    kind;
        std::string             verb;
        //std::uint32_t           bytecode[BYTECODE_MAX];
    };
    
    // MARK: - Entities
    
    struct Entity {
        enum Kind: std::uint8_t {
            Place   = 0x01,
            Thing   = 0x02
        };
        
        using RelationList = std::map<std::string, Relation::Kind>;
        
        Kind                    kind            = Place;
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
    // TODO: we should probably not have eveeeeeery thing in there? or at least
    //          clean up the architecture a bit.
    class Story {
    public:
        
        using StringPool = std::vector<std::string>;
        using VerbTable = std::map<std::string, Verb::Kind>;
        using DirectionTable = std::set<std::string>;
        using SynonymsTable = std::map<std::string, std::string>;
        
        Story();
        
        std::string uniqueID(const std::string& name) const;
        std::string uniqueID(StringID name) const;
        
        void addDirection(const std::string& dir);
        
        void addVerb(const std::string& present, Verb::Kind kind);
        optional<Verb::Kind> verb(const std::string& verb) const;
        
        std::string canonicalName(const std::string& synonym) const;
        
        // MARK: - Strings management
        StringID intern(const std::string& str) const;
        const std::string& string(StringID id) const;
        
        const StringPool& strings() const { return strings_; }
        const DirectionTable& directions() const { return directions_; }
        const VerbTable& verbs() const { return verbs_; }
        
        Context                 prototype;
        optional<std::string>   author;
        optional<std::string>   title;
    private:
        
        friend class Compiler::Sema;
        friend class Loader;
        
        // The string pool is mutable to allow const-qualified intern
        mutable StringPool      strings_;
        DirectionTable          directions_;
        VerbTable               verbs_;
        SynonymsTable           synonyms_;
    };
}
