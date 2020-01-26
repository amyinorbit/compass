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

namespace amyinorbit::compass {
    using namespace fp;

    Collector::Collector() {
        before_collection = std::mem_fn(&Collector::default_before_collection);
        after_collection = std::mem_fn(&Collector::default_after_collection);
    }

    void Collector::take(Object* obj) {
        obj->gc.next = head_;
        obj->gc.stage = !stage_;
        head_ = obj;

        roots_.push_back(obj);
        allocated_ += 1;
        if(allocated_ >= next_collection_) collect();
        roots_.pop_back();
    }

    Object* Collector::new_object(string& name, const Object* prototype) {
        auto obj = new Object(concrete_tag, prototype, name);
        take(obj);
        return obj;
    }

    Object* Collector::new_kind(string& name, const Object* prototype) {
        auto obj = new Object(concrete_tag, prototype, name);
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

        for(const auto& f: object->field_names()) {
            mark(object->field(f));
        }
    }

    void Collector::mark(const Value& value) {

        switch (value.type()) {
            case Type::object:
                mark(value.as<Value::Ref>());
                break;

            case Type::list:
                value.as<Value::Array>() | view::tapped([this](const auto& v) { mark(v); });
                break;

            default:
                break;
        }
    }

    void Collector::collect() {
        // Because we use a 'flip/flop' tag instead of a 'reachable' tag, we don't need to
        // traverse the object graph every time we collect. Win!

        // First step is marking things we know we can reach. Roots, and anything that the delegate
        // tells us about.

        allocated_ = 0;
        if(before_collection) before_collection(*this);
        roots_ | view::tapped([this](const auto& v) { mark(v); });

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
    }
}
