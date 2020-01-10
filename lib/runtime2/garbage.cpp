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
        pushRoot(other);

        allocated_ += 1;
        if(allocated_ >= nextGC_) collect();

        auto* obj = new Object(*other);
        obj->mem.next = gcHead_;
        gcHead_ = obj;

        popRoot();
        return obj;
    }

    Object* Garbage::allocate() {
        allocated_ += 1;
        if(allocated_ >= nextGC_) collect();

        auto* obj = new Object();
        obj->mem.next = gcHead_;
        gcHead_ = obj;
        return obj;
    }

    Object* Garbage::instantiate(const Object* prototype) {
        auto* obj = clone(prototype);
        obj->prototype = prototype;
        return obj;
    }

    void Garbage::markObject(const Object *obj) {
        if(!obj) return;
        if(obj->mem.mark) return;
        obj->mem.mark = true;
        allocated_ += 1;
        markObject(obj->prototype);

        for(const auto& [_, v]: obj->fields) {
            mark(v);
        }
    }

    void Garbage::mark(const Value& v) {
        if(v.is<Value::Ref>()) {
            markObject(v.as<Value::Ref>());
        }
        else if(v.is<Value::Array>()) {
            for(const auto* obj: v.as<Value::Array>()) {
                markObject(obj);
            }
        }
    }

    void Garbage::collect() {
        allocated_ = 0;

        // TODO: unmark everything
        const Object* obj = gcHead_;
        while(obj) {
            obj->mem.mark = false;
            obj = obj->mem.next;
        }

        for(auto obj: roots_) { markObject(obj); }
        if(onGC_) onGC_();

        auto** ptr = &gcHead_;
        while(*ptr) {
            if(!(*ptr)->mem.mark) {
                auto* garbage = *ptr;
                *ptr = garbage->mem.next;
                delete garbage;
            } else {
                (*ptr)->mem.mark = false;
                ptr = &(*ptr)->mem.next;
            }
        }

        nextGC_ = allocated_ * 2;
    }
}
