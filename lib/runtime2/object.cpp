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
#include <compass/runtime2/object.hpp>

namespace Compass::rt2 {

    template <typename T>
    inline T& as(Value& v) {
        return std::get<T>(v);
    }

    Object::Object(const Object& other)
    : kind_(other.kind_)
    , prototype_(other.prototype_)
    , properties_(other.properties_)
    , links_(other.links_)
    , verbs_(other.verbs_) {}

    int Object::mark() const {
        
        if(mark_) return 0;
        int allocated = 1;

        for(const auto& [_, p]: properties_) {
            if(p.is<Value::Ref>()) {
                allocated += p.as<Value::Ref>()->mark();
            }
            else if(p.is<Value::Array>()) {
                for(const auto* obj: p.as<Value::Array>()) {
                    allocated += obj->mark();
                }
            }
        }

        if(prototype_) {
            allocated += prototype_->mark();
        }
        return allocated;
    }

    bool Object::isa(const string& kind) const {
        const Object* obj = this;
        while(obj) {
            if(obj->kind_ == kind) return true;
            obj = obj->prototype_;
        }
        return false;
    }
}
