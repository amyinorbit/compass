//===--------------------------------------------------------------------------------------------===
// kind.cpp - Implementation of Compass class/kind.
// This source is part of the Compass Engine
//
// Created on 2019-03-02 by Amy Parent <amy@amyparent.com>
// Copyright (c) 2019 Amy Parent
// Licensed under the MIT License
// =^•.•^=
//===--------------------------------------------------------------------------------------------===
#include <cassert>
#include <algorithm>
#include <compass/type/kind.hpp>

namespace Compass::Type {
    
    UInt16 Kind::size() const {
        UInt16 count = 0;
        if(super_)
            count += super_->size();
        count += fields_.size();
        return count;
    }
    
    void Kind::addField(const String& name) {
        const auto existing = field(name);
        assert(!existing && "cannot add a field that already exists");
        fields_.push_back(Field{name});
    }
    
    optional<UInt16> Kind::field(const String& name) const {
        // TODO: we need to somehow do a recursive search through the class inheritance diagram
        UInt16 offset = super_ ? super_->size() : 0;
        
        for(UInt16 i = 0; i < fields_.size(); ++i) {
            if(fields_[i].name == name)
                return offset + i;
        }
        
        if(super_)
            return super_->field(name);
        return {};
    }
}
