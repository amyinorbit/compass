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
    void adjectives(const std::vector<string>& adj);

    void describe(const string& desc);
    void relation(const string& rel, const string& other);
    void location(const string& direction, const string& other);
    */
    InferEngine::InferEngine(Driver& driver) : driver_(driver) {
        gc_.onGC([this]{
            gc_.markObject(current_);
            for(const auto& [_, o]: prototypes_) { gc_.markObject(o); }
            for(const auto& [_, o]: world_) { gc_.markObject(o); }
        });

        thingKind_ = gc_.allocate();
        thingKind_->id = "Thing";
        thingKind_->field("name") = string();
        thingKind_->field("desccription") = string();
        thingKind_->verbs = {"look"};

        roomKind_ = gc_.clone(thingKind_);
        roomKind_->field("children") = Value::Array();

        prototypes_["Room"] = roomKind_;
        prototypes_["Thing"] = thingKind_;
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

    void InferEngine::refer(const string& name) {
        if(world_.count(name)) {
            current_ = world_.at(name);
            return;
        }
        current_ = gc_.allocate();
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

            current_->fields.insert(prototype->fields.begin(), prototype->fields.end());
            current_->verbs.insert(prototype->verbs.begin(), prototype->verbs.end());
        } else {
            current_->prototype = gc_.clone(current_);
            prototypes_[kind] = gc_.clone(current_);
        }
    }

    /*
    void adjectives(const std::vector<string>& adj);

    void describe(const string& desc);
    void relation(const string& rel, const string& other);
    void location(const string& direction, const string& other);
    */

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
