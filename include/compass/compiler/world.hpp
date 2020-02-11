//===--------------------------------------------------------------------------------------------===
// world.hpp - Data structure used to represent the world.
//
// Created by Amy Parent <amy@amyparent.com>
// Copyright (c) 2019 Amy Parent
// Licensed under the MIT License
// =^•.•^=
//===--------------------------------------------------------------------------------------------===
#pragma once
#include <compass/types.hpp>
#include <boost/bimap.hpp>

namespace amyinorbit::compass {
    using boost::bimap;
    struct Object;

    struct World {

        bool exists(const string& name) const { return names.count(name.lowercased()) > 0; }

        Object* object(const string& name) const {
            auto id = name.lowercased();
            return objects.count(id) ? objects.at(id) : nullptr;
        }

        Object* kind(const string& name) const {
            auto id = name.lowercased();
            return kinds.count(id) ? kinds.at(id) : nullptr;
        }

        Object* direction(const string& name) const {
            auto id = name.lowercased();
            return directions.count(id) ? directions.at(id) : nullptr;
        }

        bimap<string, string> plurals;
        map<string, Object*> kinds;
        map<string, Object*> objects;
        map<string, Object*> directions;
        set<string> names;
    };
}
