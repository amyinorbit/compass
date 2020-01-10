//===--------------------------------------------------------------------------------------------===
// object.hpp - Base interface of Compass language objects
// This source is part of the Compass Engine
//
// Created on 2019-03-03 by Amy Parent <amy@amyparent.com>
// Copyright (c) 2019 Amy Parent
// Licensed under the MIT License
// =^•.•^=
//===--------------------------------------------------------------------------------------------===
#pragma once
#include <compass/runtime2/primitives.hpp>
#include <compass/runtime2/value.hpp>
#include <iostream>

namespace amyinorbit::compass {

    struct Object;

    struct Link {
        Object* target;
        string direction;
    };

    struct Object {
        mutable struct {
            bool mark = false;
            Object* next = nullptr;
        } mem;

        string id;

        const Object* prototype = nullptr;
        map<string, Value> fields;
        vector<Link> links;
        set<string> verbs;

        bool is(const Object* proto) const;
        bool hasVerb(const string& verb) const { return verbs.count(verb); }
        Value& field(const string& field) { return fields[field]; }
        const Value& field(const string& field) const { return fields.at(field); }
    };

    Object* follow(const Object* obj, const string& to);
    void print(const Object* obj, std::ostream& out);
    
    inline std::ostream& operator << (std::ostream& left, const Object& right) {
        print(&right, left);
        return left;
    }
}
