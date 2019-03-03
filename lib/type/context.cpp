//===--------------------------------------------------------------------------------------------===
// context.cpp - Implementation of base Compass Objects
// This source is part of the Compass Engine
//
// Created on 2019-03-03 by Amy Parent <amy@amyparent.com>
// Copyright (c) 2019 Amy Parent
// Licensed under the MIT License
// =^•.•^=
//===--------------------------------------------------------------------------------------------===
#include <cassert>
#include <compass/type/context.hpp>
#include <compass/type/object.hpp>

namespace Compass::Type {
    
    Context::Context() {
        // TODO: register basic kinds (thing, place)
        auto objKind = std::make_unique<Kind>("Object");
        objKind->addField("description");
        objKind->addField("name");
        objKind->addField("article");
        
        kinds_["Object"] = std::move(objKind);
        
        auto roomKind = std::make_unique<Kind>("Room", objKind.get());
        roomKind->addField("visited");
        kinds_["Room"] = std::move(roomKind);
    }
    
    Value Context::makeObject(const Kind* kind) {
        assert(kind && "Kind must be non-null");
        auto* obj = new Object(kind);
        obj->next_ = gcHead_;
        gcHead_ = obj;
        return obj;
    }
    
    const Kind* Context::kind(const String& name) const {
        const auto it = kinds_.find(name);
        assert(it != kinds_.end() && "No kind with that name");
        return it->second.get();
    }
    
    void Context::collect() {
        
    }
    
    void Context::markObject(Object* object) {
        if(object->mark_) return;
        
        object->mark_ = true;
        for(UInt16 i = 0; i < object->size_; ++i) {
            object->property(i) | match<Object*>([this](auto obj) { markObject(obj); });
        }
    }
}
