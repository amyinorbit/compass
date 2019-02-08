//===--------------------------------------------------------------------------------------------===
// compass.cpp - Implementation of the game run class
// This source is part of the Compass Engine
//
// Created on 2019-02-06 by Amy Parent <amy@amyparent.com>
// Copyright (c) 2019 Amy Parent
// Licensed under the MIT License
// =^•.•^=
//===--------------------------------------------------------------------------------------------===
#include <algorithm>
#include <cassert>
#include <compass/run.hpp>

namespace Compass {
    
    Run::Run(const Story& story) : story_(story) {
        ctx_ = story.prototype;
        current_ = ctx_.startID;
    }
    
    const Place& Run::place(const std::string& uniqueID) const {
        const auto it = ctx_.places.find(uniqueID);
        assert(it != ctx_.places.end() && "invalid place ID");
        return it->second;
    }
    
    Place& Run::place(const std::string& uniqueID) {
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
    
    void Run::go(const std::string& placeID) {
        // TODO: handle locked places
        const auto it = ctx_.places.find(placeID);
        assert(it != ctx_.places.end() && "you can't go to non-place");
        it->second.isVisited = true;
        current_ = placeID;
    }
    
    const Thing& Run::thing(const std::string& uniqueID) const {
        const auto it = ctx_.things.find(uniqueID);
        assert(it != ctx_.things.end() && "invalid thing ID");
        return it->second;
    }
    
    Thing& Run::thing(const std::string& uniqueID) {
        const auto it = ctx_.things.find(uniqueID);
        assert(it != ctx_.things.end() && "invalid thing ID");
        return it->second;
    }
    
    void Run::take(const std::string& uniqueID) {
        const auto it = ctx_.things.find(uniqueID);
        assert(it != ctx_.things.end() && "invalid thing ID");
        inventory_.insert(uniqueID);
        
        auto& things = anything(it->second.location).things;
        things.erase(uniqueID);
        it->second.location = {};
    }
    
    bool Run::has(const std::string& uniqueID) const {
        return inventory_.find(uniqueID) != inventory_.end();
    }
    
    void Run::drop(const std::string& uniqueID) {
        inventory_.erase(uniqueID);
        // TODO:    There should be a way to drop something "on something"?
        //          if nothing is given by default, should probably drop to the current room.
    }
    
    Entity& Run::anything(const std::string& uniqueID) {
        auto placesIt = ctx_.places.find(uniqueID);
        if(placesIt != ctx_.places.end()) return placesIt->second;
        auto thingsIt = ctx_.things.find(uniqueID);
        if(thingsIt != ctx_.things.end()) return thingsIt->second;
        assert(false && "you should never reach here");
    }
    
    const Entity& Run::anything(const std::string& uniqueID) const {
        auto placesIt = ctx_.places.find(uniqueID);
        if(placesIt != ctx_.places.end()) return placesIt->second;
        auto thingsIt = ctx_.things.find(uniqueID);
        if(thingsIt != ctx_.things.end()) return thingsIt->second;
        assert(false && "you should never reach here");
    }
}
