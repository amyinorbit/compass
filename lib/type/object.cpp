//===--------------------------------------------------------------------------------------------===
// object.cpp - Implementation of the object system
// This source is part of the Compass Engine
//
// Created on 2019-03-03 by Amy Parent <amy@amyparent.com>
// Copyright (c) 2019 Amy Parent
// Licensed under the MIT License
// =^•.•^=
//===--------------------------------------------------------------------------------------------===
#include <cassert>
#include <compass/type/object.hpp>

namespace Compass::Type {
    
    Object::Object(const Kind* kind) : kind_(kind), size_(kind->size()) {
        fields_ = new Value[size_];
    }
    
    Object::~Object() {
        if(fields_)
            delete [] fields_;
    }
    
    Value& Object::property(UInt16 index) {
        assert(index < size_ && "Invalid field access");
        return fields_[index];
    }
    
    const Value& Object::property(UInt16 index) const {
        assert(index < size_ && "Invalid field access");
        return fields_[index];
    }
    
    Value& Object::property(const String& name) {
        auto index = kind_->field(name);
        assert(index && "Invalid field name");
        return property(*index);
    }
    
    const Value& Object::property(const String& name) const {
        auto index = kind_->field(name);
        assert(index && "Invalid field name");
        return property(*index);
    }
}
