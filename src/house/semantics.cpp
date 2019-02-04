//===--------------------------------------------------------------------------------------------===
// semantics.cpp - Implementation of the parser semantics checker
// This source is part of the House Engine
//
// Created on 2019-02-04 by Amy Parent <amy@amyparent.com>
// Copyright (c) 2019 Amy Parent
// Licensed under the MIT License
// =^•.•^=
//===--------------------------------------------------------------------------------------------===
#include <iostream>
#include <house/semantics.hpp>

namespace House {
    
    static std::string toLower(const std::string& str) {
        std::string lower = str;
        std::transform(lower.begin(), lower.end(), lower.begin(), ::tolower);
        return lower;
    }
    
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
    
    void Semantics::makeLink(StringID from, StringID to, const std::string& direction) {
        // std::cout << "Future link: " <<  direction << "\n";
        links_.push_back(FutureLink{
            from, to, toLower(direction)
        });
    }
    
    void Semantics::resolve(Story& story) {
        
        // Check all the links first
        
        for(const auto& link: links_) {
            const auto& from = story.string(story.uniqueID(link.from));
            const auto& to = story.string(story.uniqueID(link.to));

            const auto fromIt = story.places_.find(from);
            const auto toIt = story.places_.find(to);

            if(fromIt == story.places_.end())
                error("I can't make a link from " + from + " to " + to + " because " + from + " isn't a room");
            if(toIt == story.places_.end())
                error("I can't make a link from " + from + " to " + to + " because " + to + " isn't a room");
                
            story.addLink(link.from, link.to, link.direction);
            
            if(!hasOppositeDirection(link.direction)) continue;
            story.addLink(link.to, link.from, oppositeDirection(link.direction));
        }
        
        // TODO: We should probably check objects too
        
        
    }
}
