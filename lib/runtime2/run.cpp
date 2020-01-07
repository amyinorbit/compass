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
#include <compass/runtime2/run.hpp>
#include <compass/runtime2/object.hpp>

namespace Compass::rt2 {

    Run::Run(const Context& ctx) : ctx_(ctx) {
        for(const auto& p: ctx.prototypes) {
            prototypes_[p.kind()] = clone(&p);
        }
        // auto obj = allocate("Object");
        // obj->addField<string>("description");
        // obj->addField<string>("name");
        // obj->addField<string>("article");
        // obj->addField<Object*>("parent", nullptr);
        // obj->addField<vector<Object*>>("children");
        // prototypes_["Object"] = obj;
        //
        // auto room = allocate("Room", obj);
        // room->addField<bool>("visited");
        // prototypes_["Room"] = room;
        //
        // prototypes_["Thing"] = allocate("Thing", obj);
        // prototypes_["Container"] = allocate("Container", prototype("Thing"));
    }

    Object* Run::clone(const Object* other) {
        allocated_ += 1;
        if(allocated_ >= nextGC_) collect();
        auto* obj = new Object(*other);
        obj->next_ = gcHead_;
        gcHead_ = obj;
        return obj;
    }

    Object* Run::allocate(const string& kind, const Object* prototype) {
        allocated_ += 1;
        if(allocated_ >= nextGC_) collect();

        auto* obj = prototype ? new Object(kind, prototype) : new Object(kind);
        obj->next_ = gcHead_;
        gcHead_ = obj;
        return obj;
    }

    Object* Run::allocate(const string& kind, const string& name) {
        return allocate(kind, prototype(name));
    }

    template <typename T>
    bool is(const Value& v) {
        return std::holds_alternative<T>(v);
    }

    template <typename T>
    const T& as(const Value& v) {
        return std::get<T>(v);
    }

    template <typename T>
    T& as(Value& v) {
        return std::get<T>(v);
    }

    static int mark(const Value& v) {
        using Array = vector<Object*>;
        if(is<Object*>(v)) {
            return as<Object*>(v)->mark();
        }
        else if(is<Array>(v)) {
            int marked = 0;
            for(const auto* obj: as<Array>(v)) {
                marked += obj->mark();
            }
            return marked;
        }
        return 0;
    }

    void Run::collect() {
        allocated_ = 0;

        // TODO: unmark everything
        const Object* obj = gcHead_;
        while(obj) {
            obj->mark_ = false;
            obj = obj->next_;
        }

        for(const auto& value: stack_) { mark(value); }
        for(const auto& [_, obj]: prototypes_) { allocated_ += obj->mark(); }

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
