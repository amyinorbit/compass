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
    struct Context {
        map<string, Function> functions;
        vector<Object> prototypes;
        vector<Value> constants;
    };
}
