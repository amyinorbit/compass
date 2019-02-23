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
#include <compass/runtime/run.hpp>

namespace Compass {
    
    Run::Run(const Story& story) : story_(story) {
        ctx_ = story.prototype;
        current_ = ctx_.startID;
    }
    
    const Entity& Run::entity(const std::string& uniqueID) const {
        const auto it = ctx_.entities.find(uniqueID);
        assert(it != ctx_.entities.end() && "invalid place ID");
        return it->second;
    }
    
    Entity& Run::entity(const std::string& uniqueID) {
        const auto it = ctx_.entities.find(uniqueID);
        assert(it != ctx_.entities.end() && "invalid place ID");
        return it->second;
    }
    
    Entity& Run::current() {
        const auto it = ctx_.entities.find(current_);
        assert(it != ctx_.entities.end() && "invalid place ID");
        return it->second;
    }
    
    const Entity& Run::current() const {
        const auto it = ctx_.entities.find(current_);
        assert(it != ctx_.entities.end() && "invalid place ID");
        return it->second;
    }
    
    void Run::go(const std::string& placeID) {
        // TODO: handle locked places
        const auto it = ctx_.entities.find(placeID);
        assert(it != ctx_.entities.end() && "you can't go to non-place");
        assert(it->second.kind == Entity::Place && "you can't go to a thing");
        it->second.isSeen = true;
        current_ = placeID;
    }
    
    void Run::take(const std::string& uniqueID) {
        const auto it = ctx_.entities.find(uniqueID);
        assert(it != ctx_.entities.end() && "invalid thing ID");
        assert(it->second.kind == Entity::Thing && "you can't take a place");
        inventory_.insert(uniqueID);
        
        auto& things = entity(it->second.container->id).things;
        things.erase(uniqueID);
        it->second.container = {};
    }
    
    bool Run::has(const std::string& uniqueID) const {
        return inventory_.find(uniqueID) != inventory_.end();
    }
    
    void Run::drop(const std::string& uniqueID) {
        inventory_.erase(uniqueID);
        // TODO:    There should be a way to drop something "on something"?
        //          if nothing is given by default, should probably drop to the current room.
    }
}
