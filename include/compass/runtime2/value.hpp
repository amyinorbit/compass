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
#include <compass/runtime2/primitives.hpp>
#include <iostream>
#include <variant>

namespace amyinorbit::compass {

    class Object;
    constexpr struct nil_t {} nil;

    constexpr bool operator==(nil_t, nil_t) { return false; }
    constexpr bool operator!=(nil_t, nil_t) { return true; }

    struct Value {
    using Ref = Object*;
        using Array = vector<Ref>;

        Value() : storage(nil) {}
        template <typename T> Value(const T& value) : storage(value) {}
        template <typename T> Value& operator=(const T& value) { storage = value; return *this; }

        operator bool() const;
        bool isTrue() const;

        bool operator==(const Value& other) const { return storage == other.storage; }
        bool operator!=(const Value& other) const { return storage != other.storage; }

        template <typename T> bool is() const { return std::holds_alternative<T>(storage); }
        template <typename T> T& as() { return std::get<T>(storage); }
        template <typename T> const T& as() const { return std::get<T>(storage); }

        int index() const { return storage.index(); }
        
        void print(std::ostream& out) const;

    private:
        std::variant<nil_t, bool, double, string, Ref, Array> storage;
    };

    const Value operator+(const Value& left, const Value& right);
    const Value operator-(const Value& left, const Value& right);
    const Value operator*(const Value& left, const Value& right);
    const Value operator/(const Value& left, const Value& right);
    
    inline std::ostream& operator << (std::ostream& left, const Value& right) {
        right.print(left);
        return left;
    }
}
