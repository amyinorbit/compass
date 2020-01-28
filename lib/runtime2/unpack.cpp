//===--------------------------------------------------------------------------------------------===
// unpack.cpp - Story file loader
//
// Created by Amy Parent <amy@amyparent.com>
// Copyright (c) 2019 Amy Parent
// Licensed under the MIT License
// =^•.•^=
//===--------------------------------------------------------------------------------------------===
#include <compass/runtime2/unpack.hpp>

namespace amyinorbit::compass {
    using namespace rt;

    void Loader::load() {
        if(!signature()) return;

        reader_.forward(4 * sizeof(u32));

        std::cout << std::hex;

        u32 heap_offset = reader_.read<u32>();
        std::cout << "Heap at offset 0x" << heap_offset << "\n";

        u32 globals_offset = reader_.read<u32>();
        std::cout << "Globals at offset 0x" << globals_offset << "\n";

        u32 constants_offset = reader_.read<u32>();
        std::cout << "Constants at offset 0x" << constants_offset << "\n";

        std::cout << std::dec;

        reader_.go(constants_offset);
        u16 constants_count = reader_.read<u16>();
        for(u16 i = 0; i < constants_count; ++i) {
            constant();
        }


        reader_.go(heap_offset);
        u16 heap_count = reader_.read<u16>();
        for(u16 i = 0; i < heap_count; ++i) {
            object();
        }

    }

    void Loader::constant() {
        auto tag = reader_.read<Tag>();
        switch (tag) {
        case Tag::data_utf8: utf8(); break;
        case Tag::data_list: list(); break;
        default: break;
        }
    }

    void Loader::utf8() {
        constants_.push_back(rt::Value(reader_.read_string()));
    }

    void Loader::list() {
        auto size = reader_.read<u16>();
        vector<rt::Value> l;
        for(u16 i = 0; i < size; ++i) {
            l.push_back(value());
        }
        constants_.push_back(rt::Value(l));
    }

    Value Loader::value() {
        auto tag = reader_.read<Tag>();
        switch (tag) {
            case Tag::value_int: return reader_.read<i32>();
            case Tag::value_float: return reader_.read<float>();
            case Tag::ref_nil: return nil_tag;
            case Tag::ref_string: return Value::Defer{Value::text, reader_.read<u16>()};
            case Tag::ref_list: return Value::Defer{Value::list, reader_.read<u16>()};
            case Tag::ref_object: return Value::Defer{Value::object, reader_.read<u16>()};
            default: break;
        }
        return nil_tag;
    }

    void Loader::object() {
        assert(reader_.read<Tag>() == Tag::data_object && "not an object");

        u16 prot_ref = reader_.read<u16>();
        u16 name_ref = reader_.read<u16>();

        Object::Fields fields;

    }

    bool Loader::signature() {
        const char signature[] = "CSF2";

        const char* c = signature;
        while(*c) {
            if(reader_.read<char>() != *c) return false;
            c += 1;
        }

        return true;
    }
}
