//===--------------------------------------------------------------------------------------------===
// codegen.hpp - code generator interface
//
// Created by Amy Parent <amy@amyparent.com>
// Copyright (c) 2020 Amy Parent
// Licensed under the MIT License
// =^•.•^=
//===--------------------------------------------------------------------------------------------===
#pragma once
#include <compass/runtime2/primitives.hpp>
#include <compass/runtime2/value.hpp>

namespace amyinorbit::compass {

    class ICodeGen {
    public:

        static constexpr u16 memOffset = 0x10;

        void openFunction(const string& name);
        void closeFunction();

        u16 global(const string& name);
        u16 constant(const Value& value);

    private:
    };
}