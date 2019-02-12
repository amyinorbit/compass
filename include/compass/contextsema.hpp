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
        
        void declareDirection(const string& direction, optional<string> opposite);
        void declarePlace(const Noun& name);
        void declare(const Noun& name, optional<Noun> what = {});
        
        void setKind(optional<string> entity, Entity::Kind what);
        void setKind(optional<string> entity, const string& what);
        void setProperty(optional<string> entity, Property property);
        void setDescription(const string& text);
        void addLocation(optional<string> from, const string& to, const string& direction);
        void addAbility(optional<string> entity, const string& verb);
        
        // TODO: add support for in/on
        void setContainer(optional<string> what, const string& container);
        
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
        //std::map<string, Place>         places_;
        //std::map<string, Thing>         things_;
        
        std::map<string, Entity>       entities_;
        
        Story                           story_;
        
    };
}
