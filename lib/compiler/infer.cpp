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

        world_.property("size");
        for(const auto& s: {"small", "large", "massive", "tiny"}) {
            world_.add_property_value("size", s);
        }

        // container_["children"] = world_.list_type(world_.);
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

        world_.property(ref_->obj);

        auto obj = world_.object(prototype);
        if(!obj) return;
        obj->field(ref_->obj) = type::Value::Prop{"<not set>"};
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
        world_.property(property_name);
        world_.add_property_value(property_name, value);
    }

    void InferEngine::contained(const string& how, const string& in_what) {
        if(error(!ref_, "I am not sure what you are referring to")) return;
        if(error(ref_->field, "A property of something cannot be a new kind of property")) return;

        auto object = world_.object(ref_->obj);
        if(error(!object, "I don't know anything named " + ref_->obj)) return;

        auto container = world_.object(in_what);
        if(!container) return;
        if(error(!container->has_field("children"), in_what + " is not a container")) return;

        auto& children = container->field("children");
        if(error(!children.is<type::Value::Array>(), "invalid container")) return;

        children.as<type::Value::Array>().push_back(type::Value(object));
    }

    void InferEngine::link_to(const string& direction, const string& place) {

    }

    void InferEngine::set_property(const string& value) {
        if(error(!ref_, "I am not sure what you are referring to")) return;

        auto obj = world_.object(ref_->obj);
        if(!obj) return;

        auto prop = world_.property_of(value);
        if(!prop) return;
        if(error(ref_->field && ref_->field != prop, value + " is not a " + *prop)) return;

        obj->field(*prop) = type::Value::Prop{value};
    }
}
