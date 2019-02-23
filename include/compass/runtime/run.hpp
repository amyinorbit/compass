//===--------------------------------------------------------------------------------------------===
// run.hpp - Encapsulates a story and context, used to manage a run of the game
// This source is part of the Compass Engine
//
// Created on 2019-02-06 by Amy Parent <amy@amyparent.com>
// Copyright (c) 2019 Amy Parent
// Licensed under the MIT License
// =^•.•^=
//===--------------------------------------------------------------------------------------------===
#pragma once
#include <compass/core/story.hpp>

namespace Compass::Runtime {
    class Run {
    public:
        Run(const Story& story);
        
        const Entity& entity(const std::string& uniqueID) const;
        Entity& entity(const std::string& uniqueID);
        
        Entity& current();
        const Entity& current() const;
        
        void go(const std::string& placeID);
        
        void take(const std::string& thing);
        bool has(const std::string& thing) const;
        void drop(const std::string& thing);
        
        const std::string& string(StringID id) const {
            return story_.string(id);
        }
    private:
        
        const Story&            story_;
        std::set<std::string>   inventory_;
        std::string             current_;
        Context                 ctx_;
    };
}
