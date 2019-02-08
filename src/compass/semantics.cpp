//===--------------------------------------------------------------------------------------------===
// semantics.cpp - Implementation of the parser semantics checker
// This source is part of the Compass Engine
//
// Created on 2019-02-04 by Amy Parent <amy@amyparent.com>
// Copyright (c) 2019 Amy Parent
// Licensed under the MIT License
// =^•.•^=
//===--------------------------------------------------------------------------------------------===
#include <cassert>
#include <iostream>
#include <compass/semantics.hpp>
#include <compass/utils/string.hpp>

namespace Compass {
    
    void Semantics::error(const std::string& message) {
        std::cerr << "error: " << message << "\n";
        abort();
    }
    
    void Semantics::addDirection(const std::string& fw, const std::string& bw) {
        if(directions_.find(fw) != directions_.end()) {
            error("you have already told me about a direction called " + fw);
        }
        directions_[fw] = Direction{fw, bw};
    }
    
    void Semantics::checkDirection(const std::string& direction) {
        if(directions_.find(direction) != directions_.end()) return;
        error("you are trying to use the direction " + direction + ", which I don't know about");
    }
    
    bool Semantics::hasOppositeDirection(const std::string& direction) {
        const auto it = directions_.find(direction);
        assert(it != directions_.end() && "invalid direction given for opposite check");
        return it->second.opposite.size() > 0;
    }
    
    std::string Semantics::oppositeDirection(const std::string& direction) {
        const auto it = directions_.find(direction);
        assert(it != directions_.end() && "invalid direction given for opposite");
        assert(it->second.opposite.size() && "direction has no opposite");
        return it->second.opposite;
    }
    
    void Semantics::addPlace(const Place& place) {
        if(!start_) start_ = place.id;
        places_[place.id] = place;
    }

    void Semantics::addThing(const Thing& thing) {
        things_[thing.id] = thing;
    }
    
    void Semantics::markLink(const std::string& from, const std::string& to, const std::string& direction) {
        links_.push_back(FutureLink{ from, to, String::toLower(direction) });
    }
    
    void Semantics::resolve(Story& story) {
        auto start = maybe_guard(start_, "at least one room is required");
        // Check all the links first
        for(const auto& link: links_) {
            
            const auto fromIt = places_.find(link.from);
            const auto toIt = places_.find(link.to);

            if(fromIt == places_.end()) {
                error(
                    "I can't make a link from " + link.from + " to " + link.to
                    + " because " + link.from + " isn't a room"
                );
            }
            
            if(toIt == places_.end()) {
                error(
                    "I can't make a link from " + link.from + " to " + link.to
                    + " because " + link.to + " isn't a room"
                );
            }
            
            places_.at(link.from).links.push_back(Link{link.to, link.direction});
            if(!hasOppositeDirection(link.direction)) continue;
            places_.at(link.to).links.push_back(Link{link.from, oppositeDirection(link.direction)});
        }
        
        for(const auto& pair: things_) {
            auto thing = pair.second;
            auto placesIt = places_.find(thing.location);
            if(placesIt != places_.end()) {
                placesIt->second.things.insert(thing.id);
                continue;
            }

            auto thingsIt = things_.find(thing.id);
            if(thingsIt != things_.end()) {
                thingsIt->second.things.insert(thing.id);
                continue;
            }
            error("I can't add " + story.string(thing.name) + " to an unknown place or container");
        }
        
        
        story.prototype.startID = start;
        story.prototype.places = places_;
        story.prototype.things = things_;
    }
}
