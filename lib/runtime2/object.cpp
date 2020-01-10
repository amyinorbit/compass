//===--------------------------------------------------------------------------------------------===
// object.cpp - Implementation of the object system
// This source is part of the Compass Engine
//
// Created on 2019-03-03 by Amy Parent <amy@amyparent.com>
// Copyright (c) 2019 Amy Parent
// Licensed under the MIT License
// =^•.•^=
//===--------------------------------------------------------------------------------------------===
#include <cassert>
#include <algorithm>
#include <compass/runtime2/object.hpp>

namespace amyinorbit::compass {

    bool Object::is(const Object* proto) const {
        if(proto == this) return true;
        return prototype ? prototype->is(proto) : false;
    }

    Object* follow(const Object* obj, const string& to) {
        auto it = std::find_if(obj->links.begin(), obj->links.end(), [&](const auto& l) {
            return l.direction == to || l.target->id == to;
        });
        return it != obj->links.end() ? it->target : nullptr;
    }
    
    void printProto(const Object* obj, std::ostream& out) {
        if(!obj) return;
        out << ">" << obj->id;
        printProto(obj->prototype, out);
    }
    
    void print(const Object* obj, std::ostream& out) {
        out << obj->id << ": ";
        printProto(obj->prototype, out);
        out << " {\n";
        
        for(const auto& [name, value]: obj->fields) {
            out << "  - " << name << " = ";
            value.print(out);
            out << "\n";
        }
        
        out << "}";
    }
}
