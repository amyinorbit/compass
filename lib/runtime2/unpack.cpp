//===--------------------------------------------------------------------------------------------===
// unpack.cpp - Story file loader
//
// Created by Amy Parent <amy@amyparent.com>
// Copyright (c) 2019 Amy Parent
// Licensed under the MIT License
// =^•.•^=
//===--------------------------------------------------------------------------------------------===
#include <compass/runtime2/unpack.hpp>
#include <apfun/view.hpp>
#include <cassert>

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
        collector_.pause();
        link();
        collector_.resume();
    }

    void Loader::constant() {
        std::cout << "loading constant\n";
        auto tag = reader_.read<Tag>();
        switch (tag) {
        case Tag::data_utf8: utf8(); break;
        case Tag::data_list: list(); break;
        default: break;
        }
    }

    void Loader::utf8() {
        constants_.push_back(reader_.read_string());
        std::cout << "\t" << constants_.back().as<string>() << "\n";
    }

    void Loader::list() {
        auto size = reader_.read<u16>();
        vector<rt::Value> l;
        for(u16 i = 0; i < size; ++i) {
            l.push_back(value());
        }
        constants_.push_back(l);
    }

    Value Loader::value() {
        auto tag = reader_.read<Tag>();
        Value val(nil_tag);
        switch (tag) {
            case Tag::value_int: val = reader_.read<i32>(); break;
            case Tag::value_float: val = reader_.read<float>(); break;
            case Tag::ref_nil: reader_.forward(4); break;
            case Tag::ref_string:
                val = Value::Defer{Value::text, reader_.read<u16>()};
                reader_.forward(2);
                break;
            case Tag::ref_list:
                val = Value::Defer{Value::list, reader_.read<u16>()};
                reader_.forward(2);
                break;
            case Tag::ref_object:
                val = Value::Defer{Value::object, reader_.read<u16>()};
                reader_.forward(2);
                break;
            default: break;
        }
        return val;
    }

    string Loader::name(const Value& val) const {
        assert(val.type() == Value::text);
        if(val.is<Value::Defer>()) {
            return constants_[val.as<Value::Defer>().value].as<string>();
        }
        return val.as<string>();
    }

    void Loader::object() {
        std::cout << "loading object\n";
        assert(reader_.read<Tag>() == Tag::data_object && "not an object");

        u16 prot_ref = reader_.read<u16>();
        u16 name_ref = reader_.read<u16>();

        Object::Fields fields;
        u16 field_count = reader_.read<u16>();

        for(u16 i = 0; i < field_count; ++i) {
            auto field_name = value();
            auto field_value = value();

            fields[name(field_name)] = field_value;
        }
        objects_.emplace_back(prot_ref, name_ref, std::move(fields));
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


    const Value& Loader::constant(u16 idx, Value::Type type) {
        assert(idx < constants_.size());
        const auto& v = constants_[idx];
        assert(!v.is<Value::Defer>() && v.type() == type);

        return v;
    }

    void Loader::link() {
        for(u16 i = 0; i < objects_.size(); ++i) {
            link_object(i);
        }
    }

    Object* Loader::link_object(u16 idx) {
        if(idx == 0xffff) return nullptr;
        std::cout << "[obj/" << idx << ": req]\n";
        assert(idx < objects_.size());
        auto& data = objects_[idx];

        if(!data.linked) {
            std::cout << "[obj/" << idx << ": link]\n";
            const Object* prototype = link_object(data.prototype);
            const string& name = constant<string>(data.name);
            data.linked = collector_.new_object(prototype, name);

            for(const auto& [k, v]: data.fields) {
                data.linked->field(k) = link_value(v);
            }
            std::cout << "[obj/" << idx << ": " << data.linked << "]\n";
        }
        return data.linked;
    }

    Value Loader::link_value(const Value& v) {
        if(!v.is<Value::Defer>()) return v;
        auto ref = v.as<Value::Defer>();

        switch(ref.tag) {
            case Value::nil:
            case Value::integer:
            case Value::real:
                assert(false && "value types cannot be unlinked");
                break;

            case Value::text:
            case Value::list:
                return constant(ref.value, ref.tag);

            case Value::object:
                return link_object(ref.value);
        }
        return nil_tag;
    }
}
