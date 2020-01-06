//===--------------------------------------------------------------------------------------------===
// run.hpp - runtime2's virtual machine
//
// Created by Amy Parent <amy@amyparent.com>
// Copyright (c) 2020 Amy Parent
// Licensed under the MIT License
// =^•.•^=
//===--------------------------------------------------------------------------------------------===
#pragma once
#include <compass/type/context.hpp>
#include <compass/type/primitives.hpp>

namespace Compass::rt2 {
    using namespace Type;

    class Run {
    public:
        Run(const Context& ctx) : ctx_(ctx) {}

    private:
        Object* location_ = nullptr;
        Object* lookat_ = nullptr;
        Context ctx_;

        Value stack_[256];
    };
}