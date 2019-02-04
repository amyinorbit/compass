//===--------------------------------------------------------------------------------------------===
// story.cpp - Story data store implementation
// This source is part of the House Engine
//
// Created on 2019-02-04 by Amy Parent <amy@amyparent.com>
// Copyright (c) 2019 Amy Parent
// Licensed under the MIT License
// =^•.•^=
//===--------------------------------------------------------------------------------------------===
#include <algorithm>
#include <cassert>
#include <iostream>
#include <house/story.hpp>

namespace House {
    
    static std::string toLower(const std::string& str) {
        std::string lower = str;
        std::transform(lower.begin(), lower.end(), lower.begin(), ::tolower);
        return lower;
    }

    
    StringID Story::addPlace(Place place) {
        const auto id = makeID(place.name);
        std::cout << "new place: " << string(place.name) << "\n";
        place.uniqueID = id.first;
        places_[id.second] = place;
        return id.first;
    }
    
    StringID Story::addThing(StringID entity, Thing thing) {
        const auto id = makeID(thing.name);
        thing.uniqueID = id.first;
        thing.location = entity;
        things_[id.second] = thing;
        
        const auto& e = string(entity);
        // Then we need to find the entity we're adding this to
        auto placesIt = places_.find(e);
        if(placesIt != places_.end()) {
            placesIt->second.things.push_back(id.first);
            return id.first;
        }
        
        auto thingsIt = things_.find(e);
        if(thingsIt != things_.end()) {
            thingsIt->second.things.push_back(id.first);
            return id.first;
        }
        
        // TODO: crash with semantic error here.
        return id.first;
    }
    
    Place& Story::place(StringID uniqueID) {
        return place(string(uniqueID));
    }
    
    Place& Story::place(const std::string& uniqueID) {
        const auto it = places_.find(uniqueID);
        // TODO: replace with semantic error, that's what this is.
        assert(it != places_.end() && "invalid place ID");
        return it->second;
    }
    
    Thing& Story::thing(StringID uniqueID) {
        return thing(string(uniqueID));
    }
    
    Thing& Story::thing(const std::string& uniqueID) {
        const auto it = things_.find(uniqueID);
        // TODO: replace with semantic error, that's what this is.
        assert(it != things_.end() && "invalid thing ID");
        return it->second;
    }
    
    void Story::addDirection(const std::string& dir) {
        directions_.insert(dir);
    }
    
    void Story::addLink(StringID from, StringID to, const std::string& direction) {
        const auto& fromKey = string(from);
        const auto& toKey = string(to);
        
        places_[fromKey].links.push_back(Link{to, direction});
        
        std::cout << fromKey << " -" << direction << "-> " << toKey << "\n";
    }
    
    // // MARK: - Strings management
    
    StringID Story::installString(const std::string& str) {
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
    
    StringID Story::uniqueID(const std::string& name) {
        return makeID(installString(name)).first;
    }

    StringID Story::uniqueID(StringID name) {
        return makeID(name).first;
    }

    std::pair<StringID, std::string> Story::makeID(StringID nameID) {
        const auto& name = string(nameID);
        
        const auto id = toLower(name);
        return std::make_pair(installString(id), id);
    }
}
