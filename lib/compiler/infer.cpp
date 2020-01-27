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
    using namespace sema;

    InferEngine::InferEngine(Driver& driver) : driver_(driver) {

        properties_.insert("size");
        for(const auto& s: {"small", "large", "massive", "tiny"}) {
            values_["size"] = s;
        }

        auto base = create_kind(nullptr, "object");
        base->field("name") = "";
        base->field("plural") = "";
        base->field("description") = "";

        auto direction = create_kind(nullptr, "direction");
        direction->field("name") = "";
        direction->field("opposite") = "";


        auto room = create_kind(base, "room");
        auto relation = create_kind(nullptr, "relation");
        relation->field("direction") = nullptr;
        relation->field("target") = nullptr;

        auto thing = create_kind(base, "thing");

        room->field("directions") = Array();
        room->field("children") = Array();

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
        auto prototype = kind(prototype_name);
        if(!prototype) return;

        create_kind(prototype, ref_->obj);
    }

    void InferEngine::new_property(const string& prototype) {
        if(error(!ref_, "I am not sure what you are referring to")) return;
        if(error(ref_->field, "A property of something cannot be a new kind of property")) return;

        if(!exists(ref_->obj)) create_property(ref_->obj);

        auto obj = object(prototype);
        if(!obj) return;
        obj->field(ref_->obj) = Property{"<not set>"};
    }

    void InferEngine::new_property() {
        if(error(!ref_, "I am not sure what you are referring to")) return;
        if(error(ref_->field, "A property of something cannot be a new kind of property")) return;
        if(error(properties_.count(ref_->obj), "A property")) return;
        properties_.insert(ref_->obj);
    }

    void InferEngine::set_kind(const string& kind_name) {
        if(error(!ref_, "I am not sure what you are referring to")) return;
        if(error(ref_->field, "A property of something cannot be a new kind of property")) return;
        auto new_kind = kind(kind_name);
        if(!new_kind) return;

        if(exists(ref_->obj)) {
            auto obj = object(ref_->obj);
            if(error(!obj->set_prototype(new_kind), "I can't set the kind to " + kind_name)) return;
        } else {
            create_object(new_kind, ref_->obj);
        }
    }

    void InferEngine::declare_property(const string& property_name, const string& value) {
        if(!exists(property_name)) {
            create_property(property_name);
        }

        if(!ensure_not_exists(value)) return;
        values_[value] = property_name;
        world_[value] = Property{value};
    }

    void InferEngine::contained(const string& how, const string& in_what) {
        if(error(!ref_, "I am not sure what you are referring to")) return;
        if(error(ref_->field, "A property of something cannot be a new kind of property")) return;

        auto containee = object(ref_->obj);
        if(!containee) return;

        auto container = object(in_what);
        if(!container) return;

        if(error(!container->has_field("children"), in_what + " is not a container")) return;

        auto& children = container->field("children");
        if(error(!children.is<Array>(), "invalid container")) return;

        children.as<Array>().push_back(Value(containee));
    }

    void InferEngine::link_to(const string& direction, const string& place) {

    }

    void InferEngine::set_property(const string& value) {
        if(error(!ref_, "I am not sure what you are referring to")) return;

        auto obj = object(ref_->obj);
        if(!obj) return;

        auto prop = property_of(value);
        if(prop) {
            if(error(ref_->field && ref_->field != prop, value + " is not a " + *prop)) return;
            obj->field(*prop) = Property{value};
        } else {
            if(error(!ref_->field, "I cannot make an object into text")) return;
            obj->field(*ref_->field) = value;
        }
    }


    bool InferEngine::ensure_not_exists(const string& name) {
        if(exists(name)) {
            driver_.diagnostic(Diagnostic::error(name + " already refers to something"));
            return false;
        }
        return true;
    }

    Object* InferEngine::object(const string& name) {
        if(error(!objects_.count(name), "There are no objects called " + name)) return nullptr;
        return objects_.at(name).get();
    }

    Object* InferEngine::kind(const string& name) {
        if(error(!kinds_.count(name), "There are no objects called " + name)) return nullptr;
        return kinds_.at(name).get();
    }

    maybe<string> InferEngine::property_of(const string& value) const {
        if(!values_.count(value)) {
            driver_.diagnostic(Diagnostic::error(value + " is not something I know about"));
            return nothing();
        }
        return values_.at(value);
    }

    Object* InferEngine::create_object(const Object* proto, const string& name) {
        if(!ensure_not_exists(name)) return nullptr;

        objects_[name] = std::make_unique<Object>(proto, name);
        world_[name] = objects_.at(name).get();
        return objects_.at(name).get();
    }

    Object* InferEngine::create_kind(const Object* proto, const string& name) {
        if(!ensure_not_exists(name)) return nullptr;

        kinds_[name] = std::make_unique<Object>(proto, name);
        world_[name] = kinds_.at(name).get();
        return kinds_.at(name).get();
    }

    void InferEngine::create_property(const string& name) {
        if(!ensure_not_exists(name)) return;

        properties_.insert(name);
        world_[name] = Property{""};
    }
}
