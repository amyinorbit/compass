//===--------------------------------------------------------------------------------------------===
// infer.cpp - Inference engine implementation
//
// Created by Amy Parent <amy@amyparent.com>
// Copyright (c) 2019 Amy Parent
// Licensed under the MIT License
// =^•.•^=
//===--------------------------------------------------------------------------------------------===
#include <compass/compiler/infer.hpp>

namespace amyinorbit::compass {

    InferEngine::InferEngine(Driver& driver) : driver_(driver), world_(driver) {

        auto size = world_.property("size");
        for(const auto& s: {"small", "large", "massive", "tiny"}) {
            world_.add_property_value(size, s);
        }
    }

    void InferEngine::select(const string& what) {
        ref_ = Ref{what, nothing()};
    }

    void InferEngine::select(const string& what, const string& field) {
        ref_ = Ref{what, field};
    }


    void InferEngine::new_kind(const string& prototype_name) {
        if(error(!ref_, "I am not sure what you are referring to")) return;
        if(error(ref_->field, "A property of something cannot be a new kind")) return;
        auto prototype = world_.object(prototype_name);
        if(error(!prototype, prototype_name + " is not a kind of thing I know about")) return;
        world_.new_kind(ref_->obj, prototype);
    }

    void InferEngine::new_property(const string& prototype) {
        if(error(!ref_, "I am not sure what you are referring to")) return;
        if(error(ref_->field, "A property of something cannot be a new kind of property")) return;

        auto prop = world_.property(ref_->obj);
        if(!prop) return;

        auto obj = world_.object(prototype);
        if(!obj) return;
        obj->field(prop->name) = {prop, type::Value::Prop{"<not set>"}};
    }

    void InferEngine::new_property() {
        if(error(!ref_, "I am not sure what you are referring to")) return;
        if(error(ref_->field, "A property of something cannot be a new kind of property")) return;
        world_.property(ref_->obj);
    }

    void InferEngine::set_kind(const string& kind_name) {
        if(error(!ref_, "I am not sure what you are referring to")) return;
        if(error(ref_->field, "A property of something cannot be a new kind of property")) return;
        auto kind = world_.object(kind_name);
        if(!kind) return;
        if(error(!kind->is_abstract(), kind_name + " is not a valid kind of thing")) return;

        auto obj = world_.fetch_or_create(ref_->obj);
        error(!obj->set_kind(kind), "invalid kind name");
    }

    void InferEngine::declare_property(const string& property_name, const string& value) {
        auto property = world_.property(property_name);
        if(!property) return;
        world_.add_property_value(property, value);
    }

    void InferEngine::contained(const string& how, const string& in_what) {
        auto container = world_.object(in_what);
        if(!container) return;
        if(error(!container->is_kind("container"), in_what + "is not a container"))
    }

    void InferEngine::link_to(const string& direction, const string& place) {

    }

    void InferEngine::set_property(const string& prop) {
        if(error(!ref_, "I am not sure what you are referring to")) return;

        auto obj = world_.object(ref_->obj);
        if(!obj) return;

        auto value = world_.property_val(prop);
        if(value.is<type::nil_t>()) return;

        auto prop_name = value.type->name;
        if(error(ref_->field && ref_->field != prop_name, prop + " is not a " + prop_name)) return;

        obj->field(value.type->name) = value;
    }
}
