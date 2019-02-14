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
        addVerb(VerbBuilder("go").past("went").participle("gone").infinitive("going").make(Verb::Go));
        addVerb(VerbBuilder("walk").make(Verb::Go));
        addVerb(VerbBuilder("look").make(Verb::Look));
        addVerb(VerbBuilder("examine").infinitive("examining").make(Verb::Look));
        addVerb(VerbBuilder("take").past("took").participle("taken").infinitive("taking").make(Verb::Take));
        addVerb(VerbBuilder("drop").past("dropped").participle("dropped").infinitive("dropping").make(Verb::Drop));
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
    
    void Story::addVerb(const Verb& verb) {
        verbs_[verb.present] = verb;
    }

    optional<std::string> Story::present(const std::string& verb) const {
        const auto it = verbs_.find(verb);
        if(it == verbs_.end()) return {};
        return it->second.present;
    }
    
    optional<std::string> Story::participle(const std::string& verb) const {
        const auto it = std::find_if(verbs_.begin(), verbs_.end(), [&](const auto& v){
            return v.second.participle == verb;
        });
        if(it == verbs_.end()) return {};
        return it->second.present;
    }
    
    optional<std::string> Story::infinitive(const std::string& verb) const {
        const auto it = std::find_if(verbs_.begin(), verbs_.end(), [&](const auto& v){
            return v.second.participle == verb;
        });
        if(it == verbs_.end()) return {};
        return it->second.present;
    }
    
    const Verb& Story::verb(const std::string& verb) const {
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
