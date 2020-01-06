//===--------------------------------------------------------------------------------------------===
// context.cpp - Implementation of base Compass Objects
// This source is part of the Compass Engine
//
// Created on 2019-03-03 by Amy Parent <amy@amyparent.com>
// Copyright (c) 2019 Amy Parent
// Licensed under the MIT License
// =^•.•^=
//===--------------------------------------------------------------------------------------------===
#include <cassert>
#include <compass/type/context.hpp>
#include <compass/type/object.hpp>

namespace Compass::Type {

    Context::Context() {
        auto obj = allocate("Object");
        obj->addField<string>("description");
        obj->addField<string>("name");
        obj->addField<string>("article");
        obj->addField<Object*>("parent", nullptr);
        obj->addField<vector<Object*>>("children");
        prototypes_["Object"] = obj;

        auto room = allocate("Room", obj);
        room->addField<bool>("visited");
        prototypes_["Room"] = room;

        prototypes_["Thing"] = allocate("Thing", obj);
        prototypes_["Container"] = allocate("Container", prototype("Thing"));
    }

    Object* Context::allocate(const string& kind) const {
        allocated_ += 1;
        if(allocated_ >= nextGC_) collect();

        auto* obj = new Object(kind);
        obj->next_ = gcHead_;
        gcHead_ = obj;
        return obj;
    }

    Object* Context::allocate(const string& kind, const Object* prototype) const {
        allocated_ += 1;
        if(allocated_ >= nextGC_) collect();

        auto* obj = new Object(kind, prototype);
        obj->next_ = gcHead_;
        gcHead_ = obj;
        return obj;
    }

    Object* Context::allocate(const string& kind, const string& name) const {
        return allocate(kind, prototype(name));
    }

    void Context::collect() const {
        allocated_ = 0;
        for(const auto* obj: roots_) {
            allocated_ += obj->mark();
        }

        for(const auto& [_, obj]: prototypes_) {
            allocated_ += obj->mark();
        }

        auto** ptr = &gcHead_;
        while(*ptr) {
            if(!(*ptr)->mark_) {
                auto* garbage = *ptr;
                *ptr = garbage->next_;
                delete garbage;
            } else {
                (*ptr)->mark_ = false;
                ptr = &(*ptr)->next_;
            }
        }

        nextGC_ = allocated_ * 2;
    }
}
