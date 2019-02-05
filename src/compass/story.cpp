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
    
    const Place& Context::place(StringID uniqueID) const {
        const auto it = places.find(uniqueID);
        assert(it != places.end() && "invalid place ID");
        return it->second;
    }
    
    Place& Context::place(StringID uniqueID) {
        const auto it = places.find(uniqueID);
        assert(it != places.end() && "invalid place ID");
        return it->second;
    }
    
    const Thing& Context::thing(StringID uniqueID) const {
        const auto it = things.find(uniqueID);
        assert(it != things.end() && "invalid thing ID");
        return it->second;
    }
    
    Thing& Context::thing(StringID uniqueID) {
        const auto it = things.find(uniqueID);
        assert(it != things.end() && "invalid thing ID");
        return it->second;
    }
    
    // MARK: - Strings management
    
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
