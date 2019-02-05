//===--------------------------------------------------------------------------------------------===
// story.cpp - Story data store implementation
// This source is part of the Compass Engine
//
// Created on 2019-02-04 by Amy Parent <amy@amyparent.com>
// Copyright (c) 2019 Amy Parent
// Licensed under the MIT License
// =^•.•^=
//===--------------------------------------------------------------------------------------------===
#include <algorithm>
#include <cassert>
#include <iostream>
#include <compass/story.hpp>

namespace Compass {
    
    static std::string toLower(const std::string& str) {
        std::string lower = str;
        for(auto& c: lower) c = ::tolower(c);
        return lower;
    }

    
    // StringID Story::addPlace(Place place) {
    //     const auto id = uniqueID(place.name);
    //     place.uniqueID = id;
    //     if(!places_.size()) start = id;
    //
    //     places_[id] = place;
    //     return id;
    // }
    //
    // StringID Story::addThing(StringID entity, Thing thing) {
    //     const auto id = uniqueID(thing.name);
    //     thing.uniqueID = id;
    //     thing.location = entity;
    //     things_[id] = thing;
    //
    //     // Then we need to find the entity we're adding this to
    //     auto placesIt = places_.find(entity);
    //     if(placesIt != places_.end()) {
    //         placesIt->second.things.push_back(id);
    //         return id;
    //     }
    //
    //     auto thingsIt = things_.find(entity);
    //     if(thingsIt != things_.end()) {
    //         thingsIt->second.things.push_back(id);
    //         return id;
    //     }
    //     // TODO: crash with semantic error here.
    //     return id;
    // }
    //
    const Place& Context::place(StringID uniqueID) const {
        const auto it = places_.find(uniqueID);
        assert(it != places_.end() && "invalid place ID");
        return it->second;
    }
    
    Place& Context::place(StringID uniqueID) {
        const auto it = places_.find(uniqueID);
        assert(it != places_.end() && "invalid place ID");
        return it->second;
    }
    
    const Thing& Context::thing(StringID uniqueID) const {
        const auto it = things_.find(uniqueID);
        assert(it != things_.end() && "invalid thing ID");
        return it->second;
    }
    
    Thing& Context::thing(StringID uniqueID) {
        const auto it = things_.find(uniqueID);
        // TODO: replace with semantic error, that's what this is.
        assert(it != things_.end() && "invalid thing ID");
        return it->second;
    }
    
    // // MARK: - Strings management
    
    StringID Story::stringID(const std::string& str) const {
        const auto it = std::find(strings_.begin(), strings_.end(), str);
        if(it != strings_.end())
            return it - strings_.begin();
        
        const auto id = strings_.size();
        strings_.push_back(str);
        return id;
    }
    
    const std::string& Story::string(StringID id) const {
        assert(id < strings_.size() && "Invalid string ID");
        return strings_[id];
    }
    
    StringID Story::uniqueID(const std::string& name) const {
        const auto id = toLower(name);
        return stringID(id);
    }

    StringID Story::uniqueID(StringID name) const {
        return uniqueID(string(name));
    }
}
