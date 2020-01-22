//===--------------------------------------------------------------------------------------------===
// codegen.cpp - Code generation utilities
//
// Created by Amy Parent <amy@amyparent.com>
// Copyright (c) 2019 Amy Parent
// Licensed under the MIT License
// =^•.•^=
//===--------------------------------------------------------------------------------------------===
#include <compass/compiler/codegen.hpp>
#include <cassert>
#include <string>

namespace amyinorbit::compass {
    using namespace type;

    u32 aligned(u32 address) {
        return address + (address % 4);
    }

    u32 CodeGen::const_string(const string& str) {
        if(strings_.count(str)) return strings_.at(str).address;

        u32 address = strings_size;
        StringData data{address, (u32)str.size() + 1};
        strings_[str] = data;
        strings_size += aligned(data.size) + 8;
        return address;
    }

    u32 CodeGen::const_object(const Object* obj) {
        if(objects_.count(obj)) return objects_.at(obj).address;

        u32 address = objects_size;
        ObjectData data{address, obj->field_names()};

        objects_[obj] = data;
        objects_size += 12 + 4 * (data.fields.size());
        return address;
    }

    void CodeGen::write() {

    }

    void CodeGen::write_ptr(u8 tag, u32 value) {
        u32 data = (value & ~0b11) | (tag & 0b11);
        writer_.write<u32>(data);
    }

    void CodeGen::write_value(const Value &value) {
        switch(value.type()) {
            case Type::nil:
            write_ptr(object_tag, 0);
            break;

            case Type::number:
            writer_.write<i32>(value.as<i32>());
            break;

            case Type::text:
            write_ptr(string_tag, strings_offset() + const_string(value.as<string>()));
            // u64 size = value.as<string>().size() + 1;
            // writer_.write<u32>(size & 0xffffffff);
            // writer_.write<u32>(size >> 32);
            // writer_.write(value.as<string>().data(), size);
            break;

            case Type::object:
            if(value.as<Value::Ref>()) {
                u32 address = const_object(value.as<Value::Ref>());
                write_ptr(object_tag, objects_offset() + address);
            } else {
                write_ptr(object_tag, 0);
            }
            break;

            default:
            break;
        }
    }

    void CodeGen::write_object(const Object* obj) {
        assert(objects_.count(obj) && "object was not added to the pool");

        const auto& data = objects_.at(obj);
        u32 name = const_string(obj->name());

        write_ptr(string_tag, strings_offset() + name);
        writer_.write<u32>(data.fields.size());

        // TODO: We should really be keeping track of field indices here
        for(const auto& k: data.fields) {
            write_value(obj->field(k));
        }
    }
}
