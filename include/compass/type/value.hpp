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
#include <compass/type/primitives.hpp>

namespace Compass::Type {
    
    class Object;
    using Value = std::variant<Int, Float, String, Object*>;
    
    template <typename T>
    struct match {
        match(std::function<void(T)> fn) : fn(fn) {}
        std::function<void(T)> fn;
    };

    template <typename T>
    const Value& operator|(const Value& lhs, const match<T>& rhs) {
        if(std::holds_alternative<T>(lhs)) rhs.fn(std::get<T>(lhs));
        return lhs;
    }
    
}
