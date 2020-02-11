//===--------------------------------------------------------------------------------------------===
// sema.hpp - Semantics of the compass 2.0 type system
//
// Created by Amy Parent <amy@amyparent.com>
// Copyright (c) 2019 Amy Parent
// Licensed under the MIT License
// =^•.•^=
//===--------------------------------------------------------------------------------------------===
#pragma once
#include <compass/types.hpp>
#include <variant>

namespace amyinorbit::compass {

    struct Object;
    struct Value;

    constexpr struct nil_t {} nil_tag;

    using Ref = Object*;
    using Array = vector<Value>;

    struct Property { string value; };

    struct Value {

        enum Type { nil, integer, real, text, property, object, list };

        Value() : data_(nil_tag) {}
        Type type() const { return static_cast<Type>(data_.index()); }

        template <typename T> explicit Value(const T& val) : data_(val) {}
        template <typename T> Value& operator=(const T& val) { data_ = val; return *this; }

        template <typename T> bool is() const { return std::holds_alternative<T>(data_); }
        template <typename T> const T& as() const { return std::get<T>(data_); }
        template <typename T> T& as() { return std::get<T>(data_); }

    private:
        std::variant<nil_t, i32, float, string, Property, Ref, Array> data_;
    };

    bool operator==(const Value& left, const Value& right);
    bool operator!=(const Value& left, const Value& right);

    std::ostream& operator<<(std::ostream& out, const Value& v);

    /*
    This is not optimal at all. We basically have two versions of Object, that represent the
    same things in different layers of Compass. However, it avoids clunky mechanics at runtime
    and can make things easier in Sema + code generation
    */
    struct Object {
    public:
        using FlatRepr = map<string, Value>;

        Object(const Object* prototype, const string& name);

        const string& name() const { return name_; }
        const Object* prototype() const { return prototype_; }

        bool set_prototype(const Object* prototype);
        bool is_a(const Object* kind) const;

        bool has_field(const string& name) const;
        Value& field(const string& name);
        const Value& field(const string& name) const;
        FlatRepr flattened() const;

        void dump(std::ostream& out, int level = 0) const;

    private:

        void flatten(FlatRepr& flat) const;
        const Value* field_ptr(const string& name) const;

        const Object* prototype_;
        string name_;
        map<string, Value> fields_;
    };
}
