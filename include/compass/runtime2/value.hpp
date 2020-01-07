//===--------------------------------------------------------------------------------------------===
// value.hpp - Main interface into the Compass internal type system
// This source is part of the Compass Engine
//
// Created on 2019-03-02 by Amy Parent <amy@amyparent.com>
// Copyright (c) 2019 Amy Parent
// Licensed under the MIT License
// =^•.•^=
//===--------------------------------------------------------------------------------------------===
#pragma once
#include <variant>
#include <compass/runtime2/primitives.hpp>

namespace Compass::rt2 {

    class Object;
    struct nil_t {} nil;

    using Value = std::variant<nil_t, bool, double, string, Object*, vector<Object*>>;

    template <typename T>
    struct match {
        match(std::function<void(const T&)> fn) : fn(fn) {}
        std::function<void(const T&)> fn;
    };

    template <typename T>
    const Value& operator|(const Value& lhs, const match<T>& rhs) {
        if(std::holds_alternative<T>(lhs)) rhs.fn(std::get<T>(lhs));
        return lhs;
    }

}
