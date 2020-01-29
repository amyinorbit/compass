//===--------------------------------------------------------------------------------------------===
// unpack.hpp - Story file deserialiser
//
// Created by Amy Parent <amy@amyparent.com>
// Copyright (c) 2019 Amy Parent
// Licensed under the MIT License
// =^•.•^=
//===--------------------------------------------------------------------------------------------===
#pragma once
#include <compass/types.hpp>
#include <compass/runtime2/type.hpp>
#include <compass/runtime2/collector.hpp>
#include <compass/runtime2/bin_io.hpp>
#include <iostream>
#include <cassert>

namespace amyinorbit::compass {

    class Loader {
    public:
        Loader(rt::Collector& collector, std::istream& in)
            : collector_(collector), reader_(in) {}

        void load();

    private:
        struct Unlinked {
            Unlinked(u16 prototype, u16 name, rt::Object::Fields&& fields)
                : linked(nullptr), prototype(prototype), name(name), fields(std::move(fields)) {}

            rt::Object* linked = nullptr;

            u16 prototype;
            u16 name;
            rt::Object::Fields fields;
        };

        string name(const rt::Value& val) const;

        bool signature();
        void object();
        void constant();
        rt::Value value();

        void utf8();
        void list();

        void link();
        rt::Object* link_object(u16 idx);
        rt::Value link_value(const rt::Value& v);

        const rt::Value& constant(u16 idx, rt::Value::Type type);

        template <typename T>
        const T& constant(u16 idx) {
            assert(idx < constants_.size());
            const auto& v = constants_[idx];
            assert(v.is<T>());
            return v.as<T>();
        }

        vector<Unlinked> objects_;
        vector<rt::Value> constants_;

        rt::Collector& collector_;
        BinaryReader reader_;
    };
}
