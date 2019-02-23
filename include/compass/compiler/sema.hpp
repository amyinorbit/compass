//===--------------------------------------------------------------------------------------------===
// sema.hpp - Context-checking semantics checker for CompassLang V2
// This source is part of the Compass Engine
//
// Created on 2019-02-12 by Amy Parent <amy@amyparent.com>
// Copyright (c) 2019 Amy Parent
// Licensed under the MIT License
// =^•.•^=
//===--------------------------------------------------------------------------------------------===
#pragma once
#include <compass/core/functional.hpp>
#include <compass/core/story.hpp>
#include <compass/core/verb.hpp>

namespace Compass::Compiler {
    
    struct Noun {
        optional<std::string> article;
        std::string text;
    };
    
    class Sema {
    public:

        using string =  std::string;
        
        enum Property { Locked, None };
        
        Sema();
        
        void setTitle(const string& title);
        void setAuthor(const string& author);
        
        void declareDirection(const string& direction, optional<string> opposite);
        void checkDirection(const std::string& direction);
        bool isDirection(const std::string& direction) const;
        bool hasOppositeDirection(const std::string& direction) const;
        string oppositeDirection(const std::string& direction) const;
        
        
        void declareVerb(const Verb& verb);
        void addVerb(optional<string> entity, const string& participle);
        
        void declare(Entity::Kind kind, const Noun& name, bool silent=false);
        
        void setProperty(optional<string> entity, Property property);
        void setDescription(const string& text);
        void addLink(const optional<string>& from, const string& to, const string& direction);
        //void addAbility(optional<string> entity, const string& verb);
        
        void setRelation(const optional<string>& what, Relation::Kind where, const string& container);
        
        result<Story> resolve();
        
    private:
        struct FutureLink { string from, to, direction; };
        struct Direction { string direction; optional<string> opposite; };
        
        void error(const string& message);
        
        result<string> get(optional<string> entity);
        
        optional<string>                current_ = {};
        optional<string>                start_ = {};
        
        std::map<string, Direction>     directions_;
        std::vector<FutureLink>         links_;
        std::map<string, Entity>        entities_;
        std::map<std::string, Verb>     verbs_;
        
        Story                           story_;
        
    };
}
