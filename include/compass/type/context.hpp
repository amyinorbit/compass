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
        
        void collect();
        
        Value makeObject(const Kind* kind);
        Value makeObject(const String& kind);
        
        const Kind* kind(const String& name) const;
        
    private:
        
        void markObject(Object* object);
        
        Object*     gcHead_     = nullptr;
        std::size_t nextGC_     = 0;
        std::size_t allocated_  = 0;
        
        Map<String, std::unique_ptr<Kind>> kinds_;
    };
}
