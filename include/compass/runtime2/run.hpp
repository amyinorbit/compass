//===--------------------------------------------------------------------------------------------===
// run.hpp - runtime2's virtual machine
//
// Created by Amy Parent <amy@amyparent.com>
// Copyright (c) 2020 Amy Parent
// Licensed under the MIT License
// =^•.•^=
//===--------------------------------------------------------------------------------------------===
#pragma once
#include <compass/runtime2/context.hpp>
#include <compass/runtime2/primitives.hpp>

namespace Compass::rt2 {

    class Run {
    public:
        Run(const Context& ctx);

    private:

        Value pop() { auto v = stack_.back(); stack_.pop_back(); return v; }
        void push(const Value& value) { stack_.push_back(value); }

        void collect();
        Object* clone(const Object* other);
        Object* allocate(const string& kind, const Object* prototype = nullptr);
        Object* allocate(const string& kind, const string& prototype);
        const Object* prototype(const string& kind) const { return prototypes_.at(kind); }

        Object* location_ = nullptr;
        Object* lookat_ = nullptr;
        const Context& ctx_;

        Object* gcHead_ = nullptr;
        u64 nextGC_ = 0;
        u64 allocated_ = 0;

        map<string, Object*> prototypes_;
        vector<Value> stack_;
    };
}
