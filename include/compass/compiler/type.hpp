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
#include <compass/language/driver.hpp>
#include <apfun/maybe.hpp>
#include <variant>
#include <memory>

/*
If we want to be able to do proper type checking, we can't just rely on the runtime type
representation (well, we can, but it's clunky). Sema types to the rescue.
*/

namespace amyinorbit::compass::type {

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

        bool conforms_to(const Contract& contract) const;

        bool has_field(const string& name) const;
        bool has_field(const string& name, Type type) const;

        const auto& fields() const { return fields_; }
        Value& field(const string& name);
        const string& name() const { return name_; }
        const Object* prototype() const { return prototype_; }
    private:
        const Type* field_type(const string& name) const;
        const Value* field_ptr(const string& name) const;
        void dump_fields(std::ostream& out) const;

        const Object* prototype_;
        bool is_abstract_;
        string name_;
        map<string, Value> fields_;
    };

    class TypeDB {
    public:
        using container = map<string, std::unique_ptr<Object>>;
        using value_type = container::value_type;
        using reference = container::reference;
        using iterator = container::iterator;
        using const_iterator = container::const_iterator;


        TypeDB(Driver& driver);

        const_iterator begin() const { return world_.begin(); }
        const_iterator end() const { return world_.end(); }

        void property(const string& name);
        void add_property_value(const string& property, const string& name);
        maybe<string> property_of(const string& value);

        Object* new_kind(const string& name, const Object* prototype);
        Object* new_object(const string& name, const Object* prototype);
        Object* object(const string& name);
        Object* fetch_or_create(const string& name);

    private:

        Driver& driver_;
        map<string, const Object*> kinds_;
        map<string, string> values_;
        set<string> properties_;
        map<string, std::unique_ptr<Object>> world_;
    };
}
