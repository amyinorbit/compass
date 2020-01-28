//===--------------------------------------------------------------------------------------------===
// collector.hpp - Garbage collector for the compass 2.0 runtime
//
// Created by Amy Parent <amy@amyparent.com>
// Copyright (c) 2019 Amy Parent
// Licensed under the MIT License
// =^•.•^=
//===--------------------------------------------------------------------------------------------===
#pragma once
#include <compass/runtime2/type.hpp>
#include <compass/runtime2/buffer.hpp>
#include <utility>

namespace amyinorbit::compass::rt {

    class Collector {
    public:
        static constexpr u16 default_collection_threshold = 64;
        static constexpr float growth_factor = 1.75;

        using Delegate = std::function<void(Collector&)>;

        Collector();

        Object* new_object(Object* prototype, const string& name);
        Object* new_object(u16 prototype, u16 name, Object::Fields&& fields);
        Object* clone(const Object* object);

        void mark(const Value& value);
        void mark(const Object* object);

        void push_root(Object* obj) { roots_.push_back(obj); }
        void pop_root() { roots_.pop_back(); }

        void pause() { is_paused_ = true; }
        void resume() { is_paused_ = false; }

        Delegate before_collection{};
        Delegate after_collection{};

    private:

        void default_before_collection() {
            std::cout << "[gc]: garbage collection starting: " << allocated_ << " objects\n";
        }

        void default_after_collection() {
            std::cout << "[gc]: garbage collection done: " << allocated_ << " objects\n";
        }

        void take(Object* obj);
        void collect();

        bool stage_ = false;
        Object* head_{nullptr};
        u16 allocated_{0};
        u16 next_collection_{64};
        bool is_paused_{false};

        buffer<Object*> roots_{64};

    };
}
