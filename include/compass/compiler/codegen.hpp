//===--------------------------------------------------------------------------------------------===
// codegen.hpp - code generator interface
//
// Created by Amy Parent <amy@amyparent.com>
// Copyright (c) 2020 Amy Parent
// Licensed under the MIT License
// =^•.•^=
//===--------------------------------------------------------------------------------------------===
#pragma once
#include <compass/types.hpp>
#include <compass/compiler/sema.hpp>
#include <compass/runtime2/bytecode.hpp>
#include <compass/runtime2/bin_io.hpp>
#include <iostream>

namespace amyinorbit::compass {
    using namespace sema;

    class CodeGen {
    public:
        using Writer = BinaryWriter;

        u16 add_constant(const Value& c);
        u16 add_object(const Object* c);
        void write(std::ostream& out);

    private:

        void write_object(Writer& out, const Object* obj);
        void write_constant(Writer& out, const Value& c);
        void write_value(Writer& out, const Value& val);

        vector<Value> constants_;
        vector<const Object*> objects_;
        map<const Object*, u16> object_map_;
    };
}
