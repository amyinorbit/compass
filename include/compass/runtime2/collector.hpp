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

namespace amyinorbit::compass {

    class Collector {
    public:

        static constexpr u16 default_collection_threshold = 64;
        static constexpr float collector_growth_factor = 1.75;

        using Delegate = std::function<void(Collector&)>;

        Collector();

        Object* new_kind(string& name, const Object* prototype);
        Object* new_object(string& name, const Object* prototype);
        Object* clone(const Object* object);


        void mark(const Value& value);
        void mark(const Object* object);

        Delegate before_collection;
        Delegate after_collection;

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

        buffer<Object*> roots_{64};

    };
}
