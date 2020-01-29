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

    Object::Object(Object* prototype, string name)
        : prototype_(prototype)
        , name_(name)
        , is_linked_(true) {
        if(prototype) {
            fields_ = prototype->fields_;
        }
    }

    Object::Object(u16 prototype, u16 name, Fields&& fields)
        : prototype_(Value::Defer{Value::object, prototype})
        , name_(Value::Defer{Value::text, name})
        , fields_(std::move(fields))
        , is_linked_(false) {
    }

    bool Object::has_field(const string& name) const {
        return fields_.count(name) != 0;
    }

    Value& Object::field(const string& name) {
        assert(fields_.count(name) && "invalid field access");
        return fields_[name];
    }

    const Value& Object::field(const string& name) const {
        assert(fields_.count(name) && "invalid field access");
        return fields_.at(name);
    }

    bool Object::is_a(const string& kind) const {
        assert(name_.is<string>());
        if(name_.as<string>() == kind) return true;
        if(!prototype().as<Object*>()) return false;
        return prototype().as<Object*>()->is_a(kind);
    }

    bool Object::is_a(const Object* kind) const {
        return is_a(kind->name().as<string>());
    }
}
