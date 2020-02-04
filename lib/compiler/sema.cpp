//===--------------------------------------------------------------------------------------------===
// sema.cpp - Semantic checker for CNL and CASM
//
// Created by Amy Parent <amy@amyparent.com>
// Copyright (c) 2019 Amy Parent
// Licensed under the MIT License
// =^•.•^=
//===--------------------------------------------------------------------------------------------===
#include <compass/compiler/sema.hpp>
#include <compass/compiler/codegen.hpp>

namespace amyinorbit::compass::sema {

    bool Sema::ensure_not_exists(const string& name) {
        if(exists(name)) {
            driver_.diagnostic(Diagnostic::error(name + " already refers to something"));
            return false;
        }
        return true;
    }

    Object* Sema::object(const string& name) {
        if(error(!objects_.count(name), "There are no objects called " + name)) return nullptr;
        return objects_.at(name).get();
    }

    Object* Sema::kind(const string& name) {
        if(error(!kinds_.count(name), "There are no objects called " + name)) return nullptr;
        return kinds_.at(name).get();
    }

    maybe<string> Sema::property_of(const string& value) const {
        if(!values_.count(value)) {
            return nothing();
        }
        return values_.at(value);
    }

    Object* Sema::create_object(const Object* proto, const string& name) {
        if(!ensure_not_exists(name)) return nullptr;

        objects_[name] = std::make_unique<Object>(proto, name);
        auto obj = objects_.at(name).get();
        if(obj->has_field("name")) obj->field("name") = name;
        if(obj->has_field("plural")) obj->field("plural") = name + "s";
        world_[name] = obj;
        return obj;
    }

    Object* Sema::create_kind(const Object* proto, const string& name) {
        if(!ensure_not_exists(name)) return nullptr;

        kinds_[name] = std::make_unique<Object>(proto, name);
        auto obj = kinds_.at(name).get();
        if(obj->has_field("name")) obj->field("name") = name;
        if(obj->has_field("plural")) obj->field("plural") = name + "s";
        world_[name] = obj;
        return obj;
    }

    void Sema::create_property(const string& name) {
        if(!ensure_not_exists(name)) return;

        properties_.insert(name);
        world_[name] = Property{""};
    }


    void Sema::create_value(const string& property, const string& value) {
        values_[value] = property;
        world_[value] = Property{value};
    }

    void Sema::write(std::ostream &out) {
        CodeGen cg;
        for(const auto& [k, obj]: objects_) {
            cg.add_object(obj.get());
        }
        for(const auto& [k, obj]: kinds_) {
            cg.add_object(obj.get());
        }

        cg.write(out);
    }
}
