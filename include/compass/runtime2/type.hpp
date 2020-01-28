//===--------------------------------------------------------------------------------------------===
// type.hpp - Runtime-side type system representation
//
// Created by Amy Parent <amy@amyparent.com>
// Copyright (c) 2019 Amy Parent
// Licensed under the MIT License
// =^•.•^=
//===--------------------------------------------------------------------------------------------===
#pragma once
#include <compass/types.hpp>
#include <apfun/maybe.hpp>
#include <variant>
#include <memory>

/*
If we want to be able to do proper type checking, we can't just rely on the runtime type
representation (well, we can, but it's clunky). Sema types to the rescue.
*/

namespace amyinorbit::compass::rt {

    struct Object;
    struct Value;

    constexpr struct nil_t {} nil_tag;

    using Ref = Object*;
    using Array = vector<Value>;

    struct Value {

        enum Type { nil, integer, real, text, object, list };

        struct Defer { Type tag; u16 value; };

        Value() : data_(nil_tag) {}

        Type type() const;

        template <typename T> Value(const T& val) : data_(val) {}
        template <typename T> Value& operator=(const T& val) { data_ = val; return *this; }

        template <typename T> bool is() const { return std::holds_alternative<T>(data_); }
        template <typename T> const T& as() const { return std::get<T>(data_); }
        template <typename T> T& as() { return std::get<T>(data_); }

    private:
        std::variant<nil_t, i32, float, string, Ref, Array, Defer> data_;
    };

    struct Object {
        using Fields = map<string, Value>;
        Object(Object* prototype, string name);
        Object(u16 prototype_id, u16 name_id, Fields&& fields);

        bool is_a(const string& name) const;
        bool is_a(const Object* kind) const;

        const auto& fields() const { return fields_; }

        bool has_field(const string& name) const;
        Value& field(const string& name);
        const Value& field(const string& name) const;

        const string& name() const { return name_.as<string>(); }
        const Object* prototype() const { return prototype_.as<Object*>(); }
    private:
        friend class Collector;

        struct Field { string name; Value value; };

        mutable struct {
            Object* next = nullptr;
            bool stage = false;
        } gc;

        Value prototype_;
        Value name_;
        // vector<Field> fields_;
        Fields fields_;
    };
}
