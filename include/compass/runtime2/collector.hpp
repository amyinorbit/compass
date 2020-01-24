//===--------------------------------------------------------------------------------------------===
// collector.hpp - Garbage collector for the compass 2.0 runtime
//
// Created by Amy Parent <amy@amyparent.com>
// Copyright (c) 2019 Amy Parent
// Licensed under the MIT License
// =^•.•^=
//===--------------------------------------------------------------------------------------------===
#pragma once
#include <compass/runtime2/type.hpp>
#include <compass/runtime2/buffer.hpp>

namespace amyinorbit::compass {

    struct Context {
        buffer<Value> constants;
        buffer<Value> stack;
        map<string, Object*> id_map;
    };

    class Collector {
    public:

        Object* allocate(string& name);
        Object* allocate(string& name, const Object* prototype);
        Object* clone(const Object* other);

        const Context& context() const { return ctx_; }
        Context& context() { return ctx_;}

    private:
        Context ctx_;
    };
}
