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

    struct ObjectData {
        u32 address;
        set<string> fields;
    };

    struct PropertyData {
        u32 address;
        string name;
        u32 value_count;
    };

    struct StringData {
        u32 address;
        u32 size;
    };

    class CodeGen {
    public:
        CodeGen(std::ostream& out) : writer_(out) {}
        void write();

        u32 const_string(const string& str);
        u32 const_object(const Object* obj);

    private:
        static constexpr u8 string_tag = 0b0000;
        static constexpr u8 property_tag = 0b0001;
        static constexpr u8 object_tag = 0b0010;
        static constexpr u8 list_tag = 0b0011;


        void write_ptr(u8 tag, u32 value);
        void write_value(const Value& value);
        void write_object(const Object* obj);

        BinaryWriter writer_;

        u32 strings_size = 0;
        u32 objects_size = 0;
        u32 properties_size = 0;

        u32 strings_offset() const { return 0; }
        u32 objects_offset() const { return strings_offset() + strings_size; }
        u32 properties_offset() const { return objects_offset() + objects_size; }

        map<const Object*, ObjectData> objects_;
        map<string, StringData> strings_;
        vector<PropertyData> properties_;
    };
}
