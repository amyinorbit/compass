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

    InferEngine::InferEngine(Driver& driver) : driver_(driver) {
        gc_.onGC([this]{
            gc_.markObject(current_);
            for(const auto& [_, o]: prototypes_) { gc_.markObject(o); }
            for(const auto& [_, o]: world_) { gc_.markObject(o); }
        });

        Object* base = gc_.allocate();
        base->field("name") = string();
        base->field("description") = string();
        base->field("adjectives") = Value::Array();

        gc_.pushRoot(base);

        thingKind_ = gc_.clone(base);
        thingKind_->id = "Thing";
        thingKind_->verbs = {"look"};

        roomKind_ = gc_.clone(base);
        roomKind_->id = "Room";
        roomKind_->field("children") = Value::Array();

        prototypes_["room"] = roomKind_;
        prototypes_["thing"] = thingKind_;

        gc_.popRoot();

        for(const auto& s: {"small", "large", "massive", "tiny"}) {
            declareProperty("size", s);
        }
    }

    void InferEngine::declareDirection(const string& dir) {
        if(error(directions_.count(dir), dir + " is already a direction")) return;
        directions_[dir] = nothing();
    }

    void InferEngine::declareDirection(const string& dir, const string& opposite) {
        if(error(directions_.count(dir), dir + " is already a direction")) return;
        directions_[dir] = opposite;
        directions_[opposite] = dir;
    }

    void InferEngine::declareProperty(const string& property, const string& value) {
        properties_.insert(property);
        auto it = values_.find(value);
        if(error(values_.count(value), "you cannot change what " + value + " is.")) return;
        values_[value] = property;
        properties_.insert(property);
    }

    void InferEngine::refer(const string& name) {
        if(world_.count(name)) {
            current_ = world_.at(name);
            return;
        }
        current_ = gc_.allocate();
        current_->id = name; // TODO: we probably need a way to sanitise names
        world_[name] = current_;
    }

    void InferEngine::kind(const string& kind) {
        if(error(!current_, "I am not sure what you are referring to")) return;

        auto it = prototypes_.find(kind);
        if(it != prototypes_.end()) {
            const Object* prototype = it->second;
            if(current_->is(prototype)) return;

            if(error(
                current_->prototype && !prototype->is(current_->prototype),
                "This is already something else")
            ) return;
            current_->prototype = prototype;
            current_->fields.insert(prototype->fields.begin(), prototype->fields.end());
            current_->verbs.insert(prototype->verbs.begin(), prototype->verbs.end());
        } else {
            prototypes_[kind] = gc_.clone(current_);
            current_->prototype = prototypes_[kind];
        }
    }

    void InferEngine::property(const string& value) {
        if(error(!current_, "I am not sure what you are referring to")) return;
        auto it = values_.find(value);
        if(it == values_.end()) {
            //current_->field("adjectives").as<Value::Array>().push_back(value);
        } else {
            auto& prop = current_->field(it->second);
            if(error(!prop.is<Enum>() && !prop.is<nil_t>(), "this is not a valid property!")) return;
            prop = Enum{value};
        }
        // if(error(it != values_.end()), "")
    }

    void InferEngine::describe(const string& desc) {
        if(error(!current_, "I am not sure what you are referring to")) return;
        current_->field("description") = desc;
    }

    void InferEngine::relation(const string& rel, const string& other) {

    }

    void InferEngine::location(const string& direction, const string& other) {
        if(error(!current_, "I am not sure what you are referring to")) return;

        auto it = world_.find(other);
        if(error(it == world_.end(), "I don't know what " + other + " is")) return;

        const Object* target = it->second;
        if(error(!target->is(roomKind_), other + " is not a place")) return;
    }
}
