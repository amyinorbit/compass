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
        
        void declareDirection();
        void declarePlace();
        void declare(const Noun& name, optional<string> what);
        
        void setProperty(optional<string> what, Property property);
        void addLocation(optional<string> from, const string& to, const string& direction);
        void addAbility(optional<string> what, const string& verb);
        
        // TODO: add support for in/on
        void setContainer(optional<string> what, const string& container);
        
        result<Story> resolve();
        
    private:
        struct FutureLink { string from, to, direction; };
        struct Direction { string direction, opposite; };
        
        void error(const string& message);
        void makeCurrent(optional<string> what);
        
        optional<string>                current_ = {};
        optional<string>                start_ = {};
        
        std::map<string, Direction>     directions_;
        
        std::vector<FutureLink>         links_;
        std::map<string, Place>         places_;
        std::map<string, Thing>         things_;
        
        Story                           story_;
        
    };
}
