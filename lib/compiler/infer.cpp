//===--------------------------------------------------------------------------------------------===
// infer.cpp - Inference engine implementation
//
// Created by Amy Parent <amy@amyparent.com>
// Copyright (c) 2019 Amy Parent
// Licensed under the MIT License
// =^•.•^=
//===--------------------------------------------------------------------------------------------===
#include <compass/compiler/infer.hpp>
#include <compass/compiler/codegen.hpp>
#include <apfun/view.hpp>

namespace amyinorbit::compass {
    using namespace sema;

    InferEngine::InferEngine(Driver& driver, sema::Sema& sema) : driver_(driver), sema_(sema) {

        sema_.create_property("size");

        for(const auto& s: {"small", "large", "massive", "tiny"}) {
            sema_.create_value("size", s);
        }

        auto base = sema_.create_kind(nullptr, "object");
        base->field("name") = "";
        base->field("plural") = "";
        base->field("description") = "";

        auto direction = sema_.create_kind(nullptr, "direction");
        direction->field("name") = "";
        direction->field("opposite") = "";


        auto room = sema_.create_kind(base, "room");
        auto relation = sema_.create_kind(nullptr, "relation");
        relation->field("direction") = nullptr;
        relation->field("target") = nullptr;

        auto thing = sema_.create_kind(base, "thing");

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
        auto prototype = sema_.kind(prototype_name);
        if(!prototype) return;

        sema_.create_kind(prototype, ref_->obj);
    }

    void InferEngine::new_property(const string& prototype) {
        if(error(!ref_, "I am not sure what you are referring to")) return;
        if(error(ref_->field, "A property of something cannot be a new kind of property")) return;

        if(!sema_.exists(ref_->obj)) sema_.create_property(ref_->obj);

        auto obj = sema_.object(prototype);
        if(!obj) return;
        obj->field(ref_->obj) = Property{"<not set>"};
    }

    void InferEngine::new_property() {
        if(error(!ref_, "I am not sure what you are referring to")) return;
        if(error(ref_->field, "A property of something cannot be a new kind of property")) return;
        // if(error(sema_.pro, "A property")) return;
        sema_.create_property(ref_->obj);
    }

    void InferEngine::new_property_value(const string& property_name) {
        if(error(!ref_, "I am not sure what you are referring to")) return;
        if(error(ref_->field, "A property of something cannot be a new value of a property")) return;
        if(!sema_.exists(property_name)) {
            sema_.create_property(property_name);
        }

        if(!sema_.ensure_not_exists(ref_->obj)) return;
        sema_.create_value(property_name, ref_->obj);
    }

    void InferEngine::set_kind(const string& kind_name) {
        if(error(!ref_, "I am not sure what you are referring to")) return;
        if(error(ref_->field, "A property of something cannot be a new kind of property")) return;
        auto new_kind = sema_.kind(kind_name);
        if(!new_kind) return;

        if(sema_.exists(ref_->obj)) {
            auto obj = sema_.object(ref_->obj);
            if(error(!obj->set_prototype(new_kind), "I can't set the kind to " + kind_name)) return;
        } else {
            sema_.create_object(new_kind, ref_->obj);
        }
    }

    void InferEngine::declare_property(const string& property_name, const string& value) {
        if(!sema_.exists(property_name)) {
            sema_.create_property(property_name);
        }

        if(!sema_.ensure_not_exists(value)) return;
        sema_.create_value(property_name, value);
    }

    void InferEngine::contained(const string& how, const string& in_what) {
        if(error(!ref_, "I am not sure what you are referring to")) return;
        if(error(ref_->field, "A property of something cannot be a new kind of property")) return;

        Object *container, *containee;
        if(!(containee = sema_.object(ref_->obj))) return;
        if(!(container = sema_.object(in_what))) return;

        if(error(!container->has_field("children"), in_what + " is not a container")) return;

        auto& children = container->field("children");
        if(error(!children.is<Array>(), "invalid container")) return;

        children.as<Array>().push_back(Value(containee));
    }

    void InferEngine::link_to(const string& direction, const string& place) {

    }

    void InferEngine::set_property(const string& value) {
        if(error(!ref_, "I am not sure what you are referring to")) return;

        auto obj = sema_.object(ref_->obj);
        if(!obj) return;

        auto prop = sema_.property_of(value);
        if(prop) {
            if(error(ref_->field && ref_->field != prop, value + " is not a " + *prop)) return;
            obj->field(*prop) = Property{value};
        } else {
            if(error(!ref_->field, "I cannot make an object into text")) return;
            obj->field(*ref_->field) = value;
        }
    }
}
