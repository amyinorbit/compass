//===--------------------------------------------------------------------------------------------===
// type.cpp - Implementation of Sema/type inference
//
// Created by Amy Parent <amy@amyparent.com>
// Copyright (c) 2020 Amy Parent
// Licensed under the MIT License
// =^•.•^=
//===--------------------------------------------------------------------------------------------===
#include <compass/compiler/type.hpp>

namespace amyinorbit::compass::type {

    Sema::Sema() {
        types_["number"] = std::make_unique<Type>(Type{Type::number, "number", nullptr});
        types_["text"] = std::make_unique<Type>(Type{Type::text, "text", nullptr});
    }

    const Type* Sema::derive(const string& name, const Object* prototype) {
        return nullptr;
    }

    const Type* Sema::type_of(const Value& value) {
        switch(value.data.index()) {
            case 0: return types_.at("number").get();
            case 1: return types_.at("text").get();
            case 2: return value.type; // TODO: replace with actual checking
            default: break;
        }
        return nullptr;
    }

    const Type* Sema::new_property(const string& name) {
        types_[name] = std::make_unique<Type>(Type{Type::property, name, nullptr});
        return types_[name].get();
    }

    void Sema::add_property_value(const Type* property, const string& name) {

    }

    const Type* Sema::property_of(const string& value) {
        if(!values_.count(value)) return nullptr;
        return values_.at(value);
    }
}
