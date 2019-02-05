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

    
    StringID Story::addPlace(Place place) {
        const auto id = uniqueID(place.name);
        place.uniqueID = id;
        if(!places_.size()) start = id;
        
        places_[id] = place;
        return id;
    }
    
    StringID Story::addThing(StringID entity, Thing thing) {
        const auto id = uniqueID(thing.name);
        thing.uniqueID = id;
        thing.location = entity;
        things_[id] = thing;
        
        // Then we need to find the entity we're adding this to
        auto placesIt = places_.find(entity);
        if(placesIt != places_.end()) {
            placesIt->second.things.push_back(id);
            return id;
        }
        
        auto thingsIt = things_.find(entity);
        if(thingsIt != things_.end()) {
            thingsIt->second.things.push_back(id);
            return id;
        }
        // TODO: crash with semantic error here.
        return id;
    }
    
    const Place& Story::place(StringID uniqueID) const {
        const auto it = places_.find(uniqueID);
        assert(it != places_.end() && "invalid place ID");
        return it->second;
    }
    
    Place& Story::place(StringID uniqueID) {
        const auto it = places_.find(uniqueID);
        assert(it != places_.end() && "invalid place ID");
        return it->second;
    }
    
    const Thing& Story::thing(StringID uniqueID) const {
        const auto it = things_.find(uniqueID);
        // TODO: replace with semantic error, that's what this is.
        assert(it != things_.end() && "invalid thing ID");
        return it->second;
    }
    
    Thing& Story::thing(StringID uniqueID) {
        const auto it = things_.find(uniqueID);
        // TODO: replace with semantic error, that's what this is.
        assert(it != things_.end() && "invalid thing ID");
        return it->second;
    }
    
    void Story::addDirection(const std::string& dir) {
        directions_.insert(dir);
    }
    
    void Story::addLink(StringID from, StringID to, const std::string& direction) {
        places_[from].links.push_back(Link{to, direction});
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
        return makeID(stringID(name)).first;
    }

    StringID Story::uniqueID(StringID name) const {
        return makeID(name).first;
    }

    std::pair<StringID, std::string> Story::makeID(StringID nameID) const {
        const auto& name = string(nameID);
        
        const auto id = toLower(name);
        return std::make_pair(stringID(id), id);
    }
}
