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
#include <compass/core/story.hpp>
#include <compass/core/string.hpp>

namespace Compass {
    
    // MARK: - Strings management
    
    Story::Story() {
    }
    
    StringID Story::intern(const std::string& str) const {
        const auto it = std::find(strings_.begin(), strings_.end(), str);
        if(it != strings_.end())
            return StringID((it - strings_.begin()) + 1);
        
        const auto id = strings_.size();
        strings_.push_back(str);
        return StringID(id+1); // We offset by to allow 0 to represent an invalid string
    }
    
    const std::string& Story::string(StringID id) const {
        id -= 1;
        assert(id < strings_.size() && "Invalid string ID");
        return strings_[id];
    }
    
    void Story::addDirection(const std::string& dir) {
        directions_.insert(dir);
    }
    
    void Story::addVerb(const std::string& present, Verb::Kind kind) {
        verbs_[present] = kind;
    }

    optional<Verb::Kind> Story::verb(const std::string& verb) const {
        const auto it = verbs_.find(verb);
        if(it == verbs_.end()) return {};
        return it->second;
    }
    
    std::string Story::canonicalName(const std::string& synonym) const {
        std::cout << "searching for synonym of " << synonym << "\n";
        const auto it = synonyms_.find(synonym);
        if(it == synonyms_.end()) return synonym;
        std::cout << "synonym was found\n";
        return it->second;
    }
    
    std::string Story::uniqueID(const std::string& name) const {
        const auto id = String::toLower(name);// TODO: there's probably some sanitising to do here.
        return id;
    }

    std::string Story::uniqueID(StringID name) const {
        return uniqueID(string(name));
    }
}
