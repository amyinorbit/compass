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
    void kind(const string& kind);
    void adjectives(const std::vector<string>& adj);
    
    void describe(const string& desc);
    void relation(const string& rel, const string& other);
    void location(const string& direction, const string& other);
    */
    InferEngine::InferEngine(Driver& driver) : driver_(driver) {
        gc_.onGC([]{
            
        });
    }
    
    void InferEngine::declareDirection(const string& dir) {
        if(directions_.count(dir)) {
            driver_.diagnostic(Diagnostic::error("this direction already exists"));
            return;
        }
        directions_[dir] = nothing();
    }
    
    void InferEngine::declareDirection(const string& dir, const string& opposite) {
        if(directions_.count(dir) || directions_.count(opposite)) {
            driver_.diagnostic(Diagnostic::error("this direction already exists"));
            return;
        }
        directions_[dir] = opposite;
        directions_[opposite] = dir;
    }
    
    void InferEngine::refer(const string& name) {
        if(world_.count(name)) {
            current_ = world_.at(name);
            return;
        }
        current_ = gc_.allocate("thing");
    }
    
    // void InferEngine::kind(const string& kind) {
    //
    // }
}
