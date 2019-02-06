//===--------------------------------------------------------------------------------------------===
// compass.cpp - Implementation of the game run class
// This source is part of the Compass Engine
//
// Created on 2019-02-06 by Amy Parent <amy@amyparent.com>
// Copyright (c) 2019 Amy Parent
// Licensed under the MIT License
// =^•.•^=
//===--------------------------------------------------------------------------------------------===
#include <compass/run.hpp>

namespace Compass {
    
    Run::Run(const Story& story) : story_(story) {
        ctx_ = story.prototype;
        current_ = ctx_.startID;
    }
    
    const Place& Run::place(StringID uniqueID) const {
        const auto it = ctx_.places.find(uniqueID);
        assert(it != ctx_.places.end() && "invalid place ID");
        return it->second;
    }
    
    Place& Run::place(StringID uniqueID) {
        const auto it = ctx_.places.find(uniqueID);
        assert(it != ctx_.places.end() && "invalid place ID");
        return it->second;
    }
    
    Place& Run::current() {
        const auto it = ctx_.places.find(current_);
        assert(it != ctx_.places.end() && "invalid place ID");
        return it->second;
    }
    
    const Place& Run::current() const {
        const auto it = ctx_.places.find(current_);
        assert(it != ctx_.places.end() && "invalid place ID");
        return it->second;
    }
    
    void Run::go(StringID placeID) {
        assert(ctx_.places.find(placeID) != ctx_.places.end() && "you can't go to non-place");
        current_ = placeID;
    }
    
    const Thing& Run::thing(StringID uniqueID) const {
        const auto it = ctx_.things.find(uniqueID);
        assert(it != ctx_.things.end() && "invalid thing ID");
        return it->second;
    }
    
    Thing& Run::thing(StringID uniqueID) {
        const auto it = ctx_.things.find(uniqueID);
        assert(it != ctx_.things.end() && "invalid thing ID");
        return it->second;
    }
    
    void Run::take(StringID thing) {
        // TODO: implement taking, remove from room, all that
    }
    
    bool Run::has(StringID thing) const {
        return inventory_.find(thing) != inventory_.end();
    }
    
    void Run::drop(StringID thing) {
        // TODO: implement taking, maybe add to current room?
    }
}
