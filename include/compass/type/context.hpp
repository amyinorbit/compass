//===--------------------------------------------------------------------------------------------===
// context.hpp - Garbage collection and runtime context for the Compass Type System
// This source is part of the Compass Engine
//
// Created on 2019-03-03 by Amy Parent <amy@amyparent.com>
// Copyright (c) 2019 Amy Parent
// Licensed under the MIT License
// =^•.•^=
//===--------------------------------------------------------------------------------------------===
#pragma once
#include <memory>
#include <compass/type/object.hpp>
#include <compass/type/primitives.hpp>
#include <compass/type/value.hpp>

namespace Compass::Type {
    class Context {
    public:
        
        Context();
        
        void collect() const;
        
        Value allocate(const Kind* kind) const;
        Value allocate(const String& kind) const;
        
        void deallocate(Object* obj) const;
        
        const Kind* kind(const String& name) const;
        
    private:
        
        void markObject(const Object* object) const;
        
        mutable Object*         gcHead_     = nullptr;
        mutable std::size_t     nextGC_     = 0;
        mutable std::size_t     allocated_  = 0;
        
        Map<String, std::unique_ptr<Kind>>  kinds_;
        Array<Object*>                      roots_;
    };
}
