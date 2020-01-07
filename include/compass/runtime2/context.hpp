//===--------------------------------------------------------------------------------------------===
// context.hpp - Garbage collection and runtime context for the Compass Type System
// This source is part of the Compass Engine
//
// Created on 2019-03-03 by Amy Parent <amy@amyparent.com>
// Copyright (c) 2019 Amy Parent
// Licensed under the MIT License
// =^•.•^=
//===--------------------------------------------------------------------------------------------===
#pragma once
#include <memory>
#include <compass/runtime2/function.hpp>
#include <compass/runtime2/object.hpp>
#include <compass/runtime2/primitives.hpp>
#include <compass/runtime2/value.hpp>

namespace Compass::rt2 {
    class Context {
    public:

        Context();
        void collect() const;

        Object* allocate(const string& kind) const;
        Object* allocate(const string& kind, const Object* prototype) const;
        Object* allocate(const string& kind, const string& prototype) const;
        const Object* prototype(const string& name) const { return prototypes_.at(name); }
        void deallocate(Object* obj) const;

    private:

        void markObject(const Object* object) const;

        mutable Object*         gcHead_     = nullptr;
        mutable u64             nextGC_     = 0;
        mutable u64             allocated_  = 0;

        map<string, Function>               functions_;
        map<string, Object*>                prototypes_;
        vector<Object*>                     roots_;
    };
}
