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

    u16 CodeGen::add_constant(const Value& c) {
        constants_.push_back(c);
        return (u16)constants_.size() - 1;
    }

    u16 CodeGen::add_object(const Object* c) {
        if(!c) return 0xffff;
        if(object_map_.count(c)) return object_map_.at(c);
        u16 idx = objects_.size();
        objects_.push_back(c);
        object_map_[c] = idx;
        return idx;
    }

    void CodeGen::write(std::ostream& out) {
        Writer writer(out);

        writer.write("CSF2", 4);

        writer.write<u32>(0xffffffff);
        writer.write<u32>(0xffffffff);
        writer.write<u32>(0xffffffff);
        writer.write<u32>(0xffffffff);

        writer.write<u32>(0xffffffff);
        writer.write<u32>(0xffffffff);
        writer.write<u32>(0xffffffff);

        // Heap
        u32 heap_offset = writer.size();
        writer.write<u16>(objects_.size());
        for(const Object* obj: objects_) {
            write_object(writer, obj);
        }

        // Globals
        u32 globals_offset = writer.size();
        writer.write<u16>(0);

        // Constant pool
        u32 constants_offset = writer.size();
        writer.write<u16>(constants_.size());
        for(const auto& val: constants_) {
            write_constant(writer, val);
        }

        writer.go(4 + 4 * sizeof(u32));
        writer.write<u32>(heap_offset);
        writer.write<u32>(globals_offset);
        writer.write<u32>(constants_offset);
    }

    /*
    ### Object

        u1          tag         0xA0
        u2          prototype   reference to prototype object
        u2          name        reference to UTF8 string
        u2          field_count number of field in item
        Value[]     fields      values of the object's fields.
    */
    void CodeGen::write_object(Writer& out, const Object* obj) {
        out.write(Tag::data_object);
        if(obj->prototype()) {
            out.write<u16>(add_object(obj->prototype()));
        } else {
            out.write<u16>(0xffff);
        }
        out.write<u16>(add_constant(Value(obj->name())));

        auto fields = obj->flattened();
        out.write<u16>(fields.size());

        for(const auto& [k, v]: fields) {
            write_value(out, Value(k));
            write_value(out, v);
        }
    }

    void CodeGen::write_constant(Writer& out, const Value& val) {
        switch(val.type()) {

            case Value::text:
                out.write(Tag::data_utf8);
                out.write(val.as<string>());
                break;

            case Value::list:
                out.write(Tag::data_list);
                {
                    const auto& list = val.as<Array>();
                    out.write<u16>(list.size());
                    for(const auto& v: list) {
                        write_value(out, v);
                    }
                }
                break;

            default:
                write_value(out, val);
                break;
        }
    }

    void CodeGen::write_value(Writer& out, const Value& val) {
        switch(val.type()) {
            case Value::nil:
                out.write(Tag::ref_nil);
                out.write<u32>(0);
                break;

            case Value::integer:
                out.write(Tag::value_int);
                out.write(val.as<i32>());
                break;

            case Value::real:
                out.write(Tag::value_float);
                out.write(val.as<float>());
                break;

            case Value::text:
                out.write(Tag::ref_string);
                out.write<u16>(add_constant(val));
                out.write<u16>(0);
                break;

            case Value::property:
                out.write(Tag::ref_string);
                out.write<u16>(add_constant(val));
                out.write<u16>(0);
                break;

            case Value::object:
                out.write(Tag::ref_object);
                out.write<u16>(add_object(val.as<Ref>()));
                out.write<u16>(0);
                break;

            case Value::list:
                out.write(Tag::ref_list);
                out.write<u16>(add_constant(val));
                out.write<u16>(0);
                break;
        }
    }
}
