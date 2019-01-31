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
    
    struct Asset {
        string          article;
        string          name;
        string          description;
    };
    
    struct Object: public Asset {
        vector<string>  adjectives;
        vector<string>  verbs;
    };
        
    struct Link: public Asset {
        string          linkID;
        string          direction;
    };
    
    struct Room: public Asset {
        string          uniqueID;
        vector<Object>  objects;
        vector<Link>    links;
    };
}
