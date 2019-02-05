//===--------------------------------------------------------------------------------------------===
// semantics.hpp - Semantics checker and link resolver for the house engine
// This source is part of the Compass Engine
//
// Created on 2019-02-04 by Amy Parent <amy@amyparent.com>
// Copyright (c) 2019 Amy Parent
// Licensed under the MIT License
// =^•.•^=
//===--------------------------------------------------------------------------------------------===
#pragma once
#include <map>
#include <string>
#include <vector>
#include <house/story.hpp>

namespace Compass {
    class Semantics {
    public:
        void addDirection(const std::string& fw, const std::string& bw);
        void checkDirection(const std::string& direction);
        std::string oppositeDirection(const std::string& direction);
        bool hasOppositeDirection(const std::string& direction);
        
        void makeLink(StringID from, StringID to, const std::string& direction);
        void resolve(Story& story);
        
    private:
        struct FutureLink {
            StringID from;
            StringID to;
            std::string direction;
        };
        
        struct Direction {
            std::string             direction;
            std::string             opposite;
        };
        
        void error(const std::string& message);
        
        std::map<std::string, Direction> directions_;
        std::vector<FutureLink> links_;
        
    };
}
