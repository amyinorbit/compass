//===--------------------------------------------------------------------------------------------===
// garbage.hpp - Garbage collector for the Compass runtime
//
// Created by Amy Parent <amy@amyparent.com>
// Copyright (c) 2019 Amy Parent
// Licensed under the MIT License
// =^•.•^=
//===--------------------------------------------------------------------------------------------===
#pragma once
#include <compass/runtime2/object.hpp>
#include <compass/runtime2/value.hpp>
#include <functional>
#include <vector>

namespace amyinorbit::compass {

    class Garbage {
    public:
        using Marker = std::function<void()>;

        void onGC(Marker m);
        void mark(const Value& value);
        void markObject(const Object* obj);

        void collect();
        Object* clone(const Object* other);
        Object* allocate(const string& kind, const Object* prototype = nullptr);

        void pushRoot(const Object* obj) { roots_.push_back(obj); }
        void popRoot() { roots_.pop_back(); }

    private:
        Marker onGC_;
        std::vector<const Object*> roots_;

        Object* gcHead_ = nullptr;
        u64 nextGC_ = 64;
        u64 allocated_ = 0;
    };
}
