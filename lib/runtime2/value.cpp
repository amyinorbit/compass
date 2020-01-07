//===--------------------------------------------------------------------------------------------===
// value.cpp - Implementation of Compass's 2.0 runtime value representation
//
// Created by Amy Parent <amy@amyparent.com>
// Copyright (c) 2019 Amy Parent
// Licensed under the MIT License
// =^•.•^=
//===--------------------------------------------------------------------------------------------===
#include <compass/runtime2/value.hpp>

namespace Compass::rt2 {
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
}
