//===--------------------------------------------------------------------------------------------===
// object.hpp - Base interface of Compass language objects
// This source is part of the Compass Engine
//
// Created on 2019-03-03 by Amy Parent <amy@amyparent.com>
// Copyright (c) 2019 Amy Parent
// Licensed under the MIT License
// =^•.•^=
//===--------------------------------------------------------------------------------------------===
#pragma once
#include <compass/type/kind.hpp>
#include <compass/type/primitives.hpp>
#include <compass/type/value.hpp>

namespace Compass::Type {
    
    // TODO: we also need to have relationships handled in the C++ world, for simplicity
    // TODO: we probably need to merge the unique ID system in there?
    class Object: NonMovable, NonCopyable {
    public:
        Object(const Kind* kind);
        ~Object();
        
        Value& property(UInt16 index);
        const Value& property(UInt16 index) const;
        
        Value& property(const String& name);
        const Value& property(const String& name) const;
        
        const UInt16 size() const { return size_; }
        
    private:
        friend class Context;
        
        mutable bool    mark_   = false;
        mutable Object* next_   = nullptr;
        
        const Kind*     kind_   = nullptr;
        const UInt16    size_   = 0;
        Value*          fields_ = nullptr;
    };
}
