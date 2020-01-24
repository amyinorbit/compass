//===--------------------------------------------------------------------------------------------===
// type.hpp - Compiler-side type system representation
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

namespace amyinorbit::compass {

    struct Object;

    constexpr struct kind_t {} kind_tag;
    constexpr struct concrete_t {} concrete_tag;
    constexpr struct nil_t {} nil_tag;


    enum class Type { nil = 0, number, text, property, object, list};

    struct Value {
        using Ref = Object*;
        using Array = vector<Value>;
        struct Prop { string value; };

        Value() : data_(nil_tag) {}
        template <typename T> explicit Value(const T& val) : data_(val) {}
        template <typename T> Value& operator=(const T& val) { data_ = val; return *this; }

        bool can_assign(const Value& other) const;

        Type type() const { return static_cast<Type>(data_.index()); }

        template <typename T> bool is() const { return std::holds_alternative<T>(data_); }
        template <typename T> const T& as() const { return std::get<T>(data_); }
        template <typename T> T& as() { return std::get<T>(data_); }

    private:
        std::variant<nil_t, std::int32_t, string, Prop, Ref, Array> data_;
    };

    using Contract = map<string, Type>;

    struct Object {
        Object(kind_t, const Object* prototype, string name);
        Object(concrete_t, const Object* prototype, string name);

        void dump(std::ostream& out) const;
        bool is_abstract() const { return is_abstract_; }
        bool set_kind(const Object* kind);
        bool is_a(const Object* kind) const;

        set<string> field_names() const;

        bool conforms_to(const Contract& contract) const;

        bool has_field(const string& name) const;
        bool has_field(const string& name, Type type) const;

        const auto& fields() const { return fields_; }
        Value& field(const string& name);
        const Value& field(const string& name) const;

        const string& name() const { return name_; }
        const Object* prototype() const { return prototype_; }
    private:
        friend class Collector;

        struct {
            Object* next;
            bool is_marked;
        } gc;

        const Type* field_type(const string& name) const;
        const Value* field_ptr(const string& name) const;
        void dump_fields(std::ostream& out) const;

        void field_names(set<string>& out) const;

        const Object* prototype_;
        bool is_abstract_;
        string name_;
        map<string, Value> fields_;
    };
}
