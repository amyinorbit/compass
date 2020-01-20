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

    struct Type;
    struct Object;

    constexpr struct kind_t {} kind_tag;
    constexpr struct concrete_t {} concrete_tag;
    constexpr struct nil_t {} nil_tag;

    struct Value {
        using Ref = Object*;
        using Array = vector<Value>;
        const Type* type;
        struct Prop { string value; };
        std::variant<nil_t, std::int32_t, string, Prop, Ref, Array> data;

        template <typename T> bool is() const { return std::holds_alternative<T>(data); }
        template <typename T> const T& as() const { return std::get<T>(data); }
        template <typename T> T& as() { return std::get<T>(data); }
    };

    struct Object {
        Object(kind_t, const Object* prototype, string name);
        Object(concrete_t, const Object* prototype, string name);

        void dump(std::ostream& out) const;

        bool set_kind(const Object* kind);

        const auto& fields() const { return fields_; }
        Value& field(const string& name);
        const string& name() const { return name_; }
        bool is_abstract() const { return is_abstract_; }
        const Object* prototype() const { return prototype_; }

        bool is_kind(const string& name) const;

    private:
        const Value* field_ptr(const string& name) const;
        void dump_fields(std::ostream& out) const;
        
        const Object* prototype_;
        bool is_abstract_;
        string name_;
        map<string, Value> fields_;
    };

    struct Type {
        enum Kind {number, text, property, object, list};
        Kind kind;
        string name;
        const Type* param = nullptr;
    };

    class TypeDB {
    public:
        using container = map<string, std::unique_ptr<Object>>;
        using value_type = container::value_type;
        using reference = container::reference;
        using iterator = container::iterator;
        using const_iterator = container::const_iterator;


        TypeDB(Driver& driver);
        const Type* type_of(const Value& value);

        const_iterator begin() const { return world_.begin(); }
        const_iterator end() const { return world_.end(); }

        const Type* property(const string& name);
        void add_property_value(const Type* property, const string& name);
        const Type* property_of(const string& value);

        Object* new_kind(const string& name, const Object* prototype);
        Object* new_object(const string& name, const Object* prototype);
        Object* object(const string& name);
        Object* fetch_or_create(const string& name);

        Value text_val(const string& value) const {
            return Value{types_.at("text").get(), value};
        }

        Value number_val(std::int32_t value) const {
            return Value{types_.at("number").get(), value};
        };

        Value property_val(const string& value) const;

        Value list_val(const Type* contained) {
            return Value{list_type(contained), Value::Array()};
        }

    private:

        const Type* list_type(const Type* contained);
        const Type* prop_type_of(const string& value);
        const Type* obj_type_of(const Object* obj);

        Driver& driver_;
        map<string, std::unique_ptr<Type>> types_;
        map<string, const Type*> values_;
        map<string, std::unique_ptr<Object>> world_;
    };
}
