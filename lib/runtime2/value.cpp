//===--------------------------------------------------------------------------------------------===
// value.cpp - Implementation of Compass's 2.0 runtime value representation
//
// Created by Amy Parent <amy@amyparent.com>
// Copyright (c) 2019 Amy Parent
// Licensed under the MIT License
// =^•.•^=
//===--------------------------------------------------------------------------------------------===
#include <compass/runtime2/value.hpp>

namespace amyinorbit::compass {
    Value::operator bool() const {
        return isTrue();
    }

    bool Value::isTrue() const {
        if(is<nil_t>()) return false;
        if(is<bool>()) return as<bool>();
        if(is<double>()) return as<double>() != 0.0;
        if(is<Ref>()) return as<Ref>() != nullptr;
        return false;
    }

    const Value operator+(const Value& left, const Value& right) {
        if(left.index() != right.index()) return nil;
        if(left.is<string>()) return left.as<string>() + right.as<string>();
        if(left.is<double>()) return left.as<double>() + right.as<double>();
        return nil;
    }

    const Value operator-(const Value& left, const Value& right) {
        if(left.index() != right.index()) return nil;
        if(left.is<double>()) return left.as<double>() - right.as<double>();
        return nil;
    }

    const Value operator*(const Value& left, const Value& right) {
        if(left.index() != right.index()) return nil;
        if(left.is<double>()) return left.as<double>() * right.as<double>();
        return nil;
    }

    const Value operator/(const Value& left, const Value& right) {
        if(left.index() != right.index()) return nil;
        if(left.is<double>()) return left.as<double>() / right.as<double>();
        return nil;
    }
}
