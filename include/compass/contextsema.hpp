//===--------------------------------------------------------------------------------------------===
// contextsema.hpp - Context-checking semantics checker for CompassLang V2
// This source is part of the Compass Engine
//
// Created on 2019-02-12 by Amy Parent <amy@amyparent.com>
// Copyright (c) 2019 Amy Parent
// Licensed under the MIT License
// =^•.•^=
//===--------------------------------------------------------------------------------------------===
#pragma once
#include <compass/story.hpp>
#include <compass/utils/functional.hpp>

namespace Compass {
    
    struct Noun {
        optional<std::string> article;
        std::string text;
    };
    
    class ContextSema {
    public:

        using string =  std::string;
        
        enum Property { Locked, None };
        
        void setTitle(const string& title);
        void setAuthor(const string& author);
        
        void declareDirection(const string& direction, optional<string> opposite);
        void checkDirection(const std::string& direction);
        bool hasOppositeDirection(const std::string& direction);
        string oppositeDirection(const std::string& direction);
        
        void declare(Entity::Kind kind, const Noun& name, bool silent=false);
        
        void setProperty(optional<string> entity, Property property);
        void setDescription(const string& text);
        void addLink(const optional<string>& from, const string& to, const string& direction);
        //void addAbility(optional<string> entity, const string& verb);
        
        // TODO: add support for in/on
        void setContainer(const optional<string>& what, const string& container);
        
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
        
        Story                           story_;
        
    };
}