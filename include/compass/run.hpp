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
        
        const Place& place(StringID uniqueID) const;
        Place& place(StringID uniqueID);
        
        Place& current();
        const Place& current() const;
        
        void go(StringID placeID);
        
        const Thing& thing(StringID uniqueID) const;
        Thing& thing(StringID uniqueID);
        
        void take(StringID thing);
        bool has(StringID thing) const;
        void drop(StringID thing);
        
        const std::string& string(StringID id) const {
            return story_.string(id);
        }
    private:
        
        const Story&        story_;
        std::set<StringID>  inventory_;
        StringID            current_;
        Context             ctx_;
    };
}
