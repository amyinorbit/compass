//===--------------------------------------------------------------------------------------------===
// collector.cpp - Garbage collector implementation
//
// Created by Amy Parent <amy@amyparent.com>
// Copyright (c) 2019 Amy Parent
// Licensed under the MIT License
// =^•.•^=
//===--------------------------------------------------------------------------------------------===
#include <compass/runtime2/collector.hpp>
#include <memory>
#include <apfun/view.hpp>

namespace amyinorbit::compass::rt {
    using namespace fp;

    Collector::Collector() {
        // before_collection = std::mem_fn(&Collector::default_before_collection);
        // after_collection = std::mem_fn(&Collector::default_after_collection);
    }

    void Collector::take(Object* obj) {
        obj->gc.next = head_;
        obj->gc.stage = !stage_;
        head_ = obj;

        roots_.push_back(obj);
        allocated_ += 1;
        if(!is_paused_ && allocated_ >= next_collection_) collect();
        roots_.pop_back();
    }

    Object* Collector::new_object(Object* prototype, const string& name) {
        auto obj = new Object(prototype, name);
        take(obj);
        return obj;
    }

    Object* Collector::new_object(Object* prototype, const string& name, Object::Fields&& fields) {
        auto obj = new Object(prototype, name, std::move(fields));
        take(obj);
        return obj;
    }

    Object* Collector::clone(const Object* object) {
        auto obj = new Object(*object);
        take(obj);
        return obj;
    }

    void Collector::mark(const Object* object) {
        if(object->gc.stage == stage_) return;
        allocated_ += 1;
        object->gc.stage = stage_;

        mark(object->prototype_);
        mark(object->name_);

        for(const auto& [_, v]: object->fields()) {
            mark(v);
        }
    }

    void Collector::mark(const Value& value) {

        switch (value.type()) {
            case Value::object:
                mark(value.as<Ref>());
                break;

            case Value::list:
                for(const auto& v: value.as<Array>())
                    mark(v);
                break;

            default: break;
        }
    }

    void Collector::collect() {
        // Because we use a 'flip/flop' tag instead of a 'reachable' tag, we don't need to
        // traverse the object graph every time we collect. Win!

        // First step is marking things we know we can reach. Roots, and anything that the delegate
        // tells us about.
        std::cout << "[gc]: garbage collection starting: " << allocated_ << " objects\n";

        allocated_ = 0;
        if(before_collection) before_collection(*this);
        for(const auto& v : roots_) mark(v);

        // Then we can nuke anything that isn't marked
        using Ref = Object*;
        Ref* head_ptr = &head_;
        while(*head_ptr) {
            Ref obj = *head_ptr;
            if(obj->gc.stage != stage_) {
                *head_ptr = obj->gc.next;
                delete obj;
            } else {
                head_ptr = &obj->gc.next;
            }
        }

        // Finally we need to flip the stage marker.
        stage_ = !stage_;
        if(after_collection) after_collection(*this);
        next_collection_ = allocated_ > default_collection_threshold
            ? allocated_ * growth_factor
            : default_collection_threshold;
        std::cout << "[gc]: garbage collection done: " << allocated_ << " objects\n";
    }
}
