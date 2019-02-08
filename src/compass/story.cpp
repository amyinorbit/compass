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
#include <compass/utils/string.hpp>

namespace Compass {
    
    // MARK: - Strings management
    
    Story::Story() {
        addVerb(Verb::Go, stringID("go"), stringID("to"));
        addVerb(Verb::Go, stringID("walk"), stringID("to"));
        addVerb(Verb::Look, stringID("look"), stringID("at"));

        addVerb(Verb::Take, stringID("take"));
        addVerb(Verb::Drop, stringID("drop"));
    }
    
    StringID Story::stringID(const std::string& str) const {
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
    
    void Story::addVerb(Verb::Kind kind, StringID verb, StringID preposition) {
        // TODO: probably do some sort of uniqueness check here
        verbs_[string(verb)] = Verb{kind, verb, preposition};
    }
    
    bool Story::isVerb(const std::string& verb) const {
        return verbs_.find(verb) != verbs_.end();
    }
    
    const Verb& Story::getVerb(const std::string& verb) const {
        const auto it = verbs_.find(verb);
        assert(it != verbs_.end() && "This is not a verb");
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
