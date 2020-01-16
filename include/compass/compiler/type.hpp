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
#include <variant>
#include <memory>

/*
If we want to be able to do proper type checking, we can't just rely on the runtime type
representation (well, we can, but it's clunky). Sema types to the rescue.
*/

namespace amyinorbit::compass::type {

    struct Type;
    struct Object;

    struct Value {
        using Ref = std::shared_ptr<Object>;
        using Array = vector<Value>;
        const Type* type;
        std::variant<std::int32_t, string, Ref, Array> data;

        template <typename T> const T& as() const { return std::get<T>(data); }
        template <typename T> T& as() { return std::get<T>(data); }
    };

    struct Object {
        const Object* prototype;
        bool is_abstract;
        string name;
        map<string, Value> fields;
    };

    struct Type {
        enum Kind {number, text, property, object, list};
        Kind kind;
        string name;
        const Type* param = nullptr;
    };

    class Sema {
    public:
        Sema();

        const Type* derive(const string& name, const Object* prototype);
        const Type* type_of(const Value& value);

        const Type* new_property(const string& name);
        void add_property_value(const Type* property, const string& name);
        const Type* property_of(const string& value);

    private:

        map<string, std::unique_ptr<Type>> types_;
        map<string, const Type*> values_;
        map<string, std::unique_ptr<Object>> world_;
    };
}
