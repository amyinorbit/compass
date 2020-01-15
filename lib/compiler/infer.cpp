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

    /*
    void contained(const string& how, const string& in_what);
    void link_to(const string& direction, const string& place);

    void set_property(const string& prop);
    */

    InferEngine::InferEngine(Driver& driver) : driver_(driver) {
        gc_.onGC([this]{
            for(const auto& [_, o]: prototypes_) { gc_.markObject(o); }
            for(const auto& [_, o]: world_) { gc_.markObject(o); }
            for(const auto& [_, o]: directions_) { gc_.markObject(o); }
        });

        Object* base = gc_.allocate();
        base->id = "Object";
        base->field("name") = string();
        base->field("plural") = nil;
        base->field("description") = string();
        base->field("adjectives") = Value::Array();

        gc_.pushRoot(base);

        thingKind_ = gc_.clone(base);
        thingKind_->id = "Thing";
        thingKind_->verbs = {"look"};

        roomKind_ = gc_.clone(base);
        roomKind_->id = "Place";
        roomKind_->field("children") = Value::Array();

        directionKind_ = gc_.clone(base);
        directionKind_->id = "Direction";
        directionKind_->field("opposite") = nullptr;

        prototypes_["place"] = roomKind_;
        prototypes_["thing"] = thingKind_;
        prototypes_["direction"] = directionKind_;

        gc_.popRoot();

        for(const auto& s: {"small", "large", "massive", "tiny"}) {
            declare_property("size", s);
        }
    }


    void InferEngine::select(const string& what) {
        ref_ = Ref{what, nothing()};
    }

    void InferEngine::select(const string& what, const string& field) {
        ref_ = Ref{what, field};
    }


    void InferEngine::new_kind(const string& prototype) {
        if(error(!ref_, "I am not sure what you are referring to")) return;
        if(error(ref_->field, "A property of something cannot be a new kind")) return;
        auto it = prototypes_.find(prototype);
        if(error(it == prototypes_.end(), prototype + " is not a kind of thing I know about"))
            return;
        Object* kind = gc_.instantiate(it->second);
        kind->id = ref_->obj;
        kind->field("name") = kind->id;
        kind->field("plural") = kind->id + "s";
        prototypes_[kind->id] = kind;
    }

    void InferEngine::new_property(const string& prototype) {
        if(error(!ref_, "I am not sure what you are referring to")) return;
        if(error(ref_->field, "A property of something cannot be a new kind of property")) return;
        auto it = prototypes_.find(prototype);
        if(error(it == prototypes_.end(), prototype + " is not a kind of thing I know about"))
            return;
        new_property();
        Object* p = it->second;
        auto& value = p->field(ref_->obj);
        if(!value.is<Enum>())
            value = Enum{""};
    }

    void InferEngine::new_property() {
        if(error(!ref_, "I am not sure what you are referring to")) return;
        if(error(ref_->field, "A property of something cannot be a new kind of property")) return;
        properties_.insert(ref_->obj);
    }

    void InferEngine::set_kind(const string& kind) {
        if(error(!ref_, "I am not sure what you are referring to")) return;
        if(error(ref_->field, "A property of something cannot be a new kind of property")) return;

        auto it = prototypes_.find(kind);
        if(error(it == prototypes_.end(), kind + " is not a thing I know of")) return;

        const Object* prototype = it->second;
        Object* obj = get_or_create(ref_->obj);

        if(obj->is(prototype)) return;
        if(error(
            obj->prototype && !prototype->is(obj->prototype),
            "This is already something else")
        ) return;
        obj->prototype = prototype;
        obj->fields.insert(prototype->fields.begin(), prototype->fields.end());
        obj->verbs.insert(prototype->verbs.begin(), prototype->verbs.end());
    }

    void InferEngine::declare_property(const string& property, const string& value) {
        properties_.insert(property);
        if(error(values_.count(value), "you cannot change what " + value + " is.")) return;
        values_[value] = property;
        properties_.insert(property);
    }


    Object* InferEngine::get_or_create(const string& name) {
        if(!world_.count(name)) {
            world_[name] = gc_.allocate();
            world_[name]->field("name") = name;
        }
        return world_.at(name);
    }


    void InferEngine::contained(const string& how, const string& in_what) {

    }

    void InferEngine::link_to(const string& direction, const string& place) {

    }

    void InferEngine::set_property(const string& prop) {
        if(error(!ref_, "I am not sure what you are referring to")) return;
        if(error(world_.count(ref_->obj) == 0, "There's nothing here called " + prop)) return;
        if(error(values_.count(prop) == 0, "I am not sure what " + prop + " is")) return;
        const auto& prop_name = values_.at(prop);
        if(error(ref_->field && ref_->field != prop_name, prop + " is not a " + prop_name)) return;

        Object* obj = world_.at(ref_->obj);
        obj->field(prop_name) = prop;
    }

    // void InferEngine::kind(const string& kind) {
    //     if(error(!current_, "I am not sure what you are referring to")) return;
    //
    //     auto it = prototypes_.find(kind);
    //     if(it != prototypes_.end()) {
    //         const Object* prototype = it->second;
    //         if(current_->is(prototype)) return;
    //
    //         if(error(
    //             current_->prototype && !prototype->is(current_->prototype),
    //             "This is already something else")
    //         ) return;
    //         current_->prototype = prototype;
    //         current_->fields.insert(prototype->fields.begin(), prototype->fields.end());
    //         current_->verbs.insert(prototype->verbs.begin(), prototype->verbs.end());
    //     } else {
    //         prototypes_[kind] = gc_.clone(current_);
    //         current_->prototype = prototypes_[kind];
    //     }
    // }
    //
    // void InferEngine::property(const string& value) {
    //     if(error(!current_, "I am not sure what you are referring to")) return;
    //     auto it = values_.find(value);
    //     if(it == values_.end()) {
    //         //current_->field("adjectives").as<Value::Array>().push_back(value);
    //     } else {
    //         auto& prop = current_->field(it->second);
    //         if(error(!prop.is<Enum>() && !prop.is<nil_t>(), "this is not a valid property!")) return;
    //         prop = Enum{value};
    //     }
    //     // if(error(it != values_.end()), "")
    // }
    //
    // void InferEngine::describe(const string& desc) {
    //     if(error(!current_, "I am not sure what you are referring to")) return;
    //     current_->field("description") = desc;
    // }
    //
    // void InferEngine::relation(const string& rel, const string& other) {
    //
    // }
    //
    // void InferEngine::location(const string& direction, const string& other) {
    //     if(error(!current_, "I am not sure what you are referring to")) return;
    //
    //     auto it = world_.find(other);
    //     if(error(it == world_.end(), "I don't know what " + other + " is")) return;
    //
    //     const Object* target = it->second;
    //     if(error(!target->is(roomKind_), other + " is not a place")) return;
    // }
}
