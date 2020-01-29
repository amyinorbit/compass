//===--------------------------------------------------------------------------------------------===
// type.cpp - Implementation of TypeDB/type inference
//
// Created by Amy Parent <amy@amyparent.com>
// Copyright (c) 2020 Amy Parent
// Licensed under the MIT License
// =^•.•^=
//===--------------------------------------------------------------------------------------------===
#include <compass/runtime2/type.hpp>
#include <cassert>

namespace amyinorbit::compass::rt {

    Value::Type Value::type() const {
        if(is<Defer>()) return as<Defer>().tag;
        return static_cast<Type>(data_.index());
    }

    Object::Object(const Object* prototype, string name)
        : prototype_(prototype)
        , name_(name)
        , is_linked_(true) {
        if(prototype) {
            fields_ = prototype->fields_;
        }
    }

    bool Object::has_field(const string& name) const {
        return fields_.count(name) != 0;
    }

    Value& Object::field(const string& name) {
        // assert(fields_.count(name) && "invalid field access");
        return fields_[name];
    }

    const Value& Object::field(const string& name) const {
        assert(fields_.count(name) && "invalid field access");
        return fields_.at(name);
    }

    bool Object::is_a(const string& kind) const {
        if(name_ == kind) return true;
        if(!prototype()) return false;
        return prototype()->is_a(kind);
    }

    bool Object::is_a(const Object* kind) const {
        return is_a(kind->name());
    }
}
