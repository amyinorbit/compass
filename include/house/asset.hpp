//===--------------------------------------------------------------------------------------------===
// asset.hpp - House adventure Asset
// This source is part of the House Engine
//
// Created on 2019-01-30 by Amy Parent <amy@amyparent.com>
// Copyright (c) 2018 Amy Parent
// Licensed under the MIT License
// =^•.•^=
//===--------------------------------------------------------------------------------------------===
#pragma once
#include <string>
#include <vector>

namespace House {
    using std::string;
    using std::vector;
    
    enum class Direction {
        North, Northeast, East, Southeast, South, Southwest,
    };
    
    struct Object {
        string          name;
        string          uniqueID;
        
        vector<string>  adjectives;
        vector<string>  verbs;
    };
        
    struct Link {
        string          name;
        string          uniqueID;
        
        string          description;
        string          direction;
    };
    
    struct Room {
        string          name;
        string          description;
        
        vector<Object>  objects;
        vector<Link>    links;
    };
}
