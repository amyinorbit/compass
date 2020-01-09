//===--------------------------------------------------------------------------------------------===
// garbage.cpp - Garbage collector implementation
//
// Created by Amy Parent <amy@amyparent.com>
// Copyright (c) 2019 Amy Parent
// Licensed under the MIT License
// =^•.•^=
//===--------------------------------------------------------------------------------------------===
#include <compass/runtime2/garbage.hpp>

namespace amyinorbit::compass {
    Object* Garbage::clone(const Object* other) {
        allocated_ += 1;
        if(allocated_ >= nextGC_) collect();
        auto* obj = new Object(*other);
        obj->next_ = gcHead_;
        gcHead_ = obj;
        return obj;
    }

    Object* Garbage::allocate(const string& kind, const Object* prototype) {
        allocated_ += 1;
        if(allocated_ >= nextGC_) collect();

        auto* obj = prototype ? new Object(kind, prototype) : new Object(kind);
        obj->next_ = gcHead_;
        gcHead_ = obj;
        return obj;
    }

    void Garbage::mark(const Value& v) {
        if(v.is<Value::Ref>()) {
            auto obj = v.as<Value::Ref>();
            allocated_ += obj->mark();
        }
        else if(v.is<Value::Array>()) {
            for(const auto* obj: v.as<Value::Array>()) {
                allocated_ += obj->mark();
            }
        }
    }

    void Garbage::collect() {
        allocated_ = 0;

        // TODO: unmark everything
        const Object* obj = gcHead_;
        while(obj) {
            obj->mark_ = false;
            obj = obj->next_;
        }

        for(auto obj: roots_) { obj->mark(); }
        if(onGC_) onGC_();

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
