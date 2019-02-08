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
#include <compass/story.hpp>

namespace Compass {
    class Run {
    public:
        Run(const Story& story);
        
        const Place& place(const std::string& uniqueID) const;
        Place& place(const std::string& uniqueID);
        
        Place& current();
        const Place& current() const;
        
        void go(const std::string& placeID);
        
        const Thing& thing(const std::string& uniqueID) const;
        Thing& thing(const std::string& uniqueID);
        
        void take(const std::string& thing);
        bool has(const std::string& thing) const;
        void drop(const std::string& thing);
        
        const std::string& string(StringID id) const {
            return story_.string(id);
        }
    private:
        
        Entity& anything(const std::string& uniqueID);
        const Entity& anything(const std::string& uniqueID) const;
        
        const Story&            story_;
        std::set<std::string>   inventory_;
        std::string             current_;
        Context                 ctx_;
    };
}
