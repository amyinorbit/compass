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
    };

    struct Object {
        const Object* prototype;
        string name;
        map<string, Value> fields;
    };

    struct Type {
        enum Kind {number, text, property, object, list};
        Kind kind;
        string name;
        const Type* param = nullptr;
    };

    class TypeDB {
    public:
        TypeDB();

    private:

        std::unique_ptr<Type> number;
        std::unique_ptr<Type> text;

        map<string, std::unique_ptr<Type>> types;
        map<string, const Type*> values;
        map<string, Value> world;
    };
}
