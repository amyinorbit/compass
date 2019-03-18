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
        objKind->addField("parent");
        objKind->addField("children");
        
        kinds_["Object"] = std::move(objKind);
        
        auto roomKind = std::make_unique<Kind>("Room", objKind.get());
        roomKind->addField("visited");
        kinds_["Room"] = std::move(roomKind);
    }
    
    Value Context::allocate(const Kind* kind) const {
        assert(kind && "Kind must be non-null");
        allocated_ += sizeof(Object);
        if(allocated_ >= nextGC_) collect();
        auto* obj = new Object(kind);
        obj->next_ = gcHead_;
        gcHead_ = obj;
        return obj;
    }
    
    Value Context::allocate(const String& name) const {
        return allocate(kind(name));
    }
    
    const Kind* Context::kind(const String& name) const {
        const auto it = kinds_.find(name);
        assert(it != kinds_.end() && "No kind with that name");
        return it->second.get();
    }
    
    void Context::deallocate(Object* obj) const {
        delete obj;
    }
    
    void Context::collect() const {
        allocated_ = 0;
        for(const auto* obj: roots_) {
            markObject(obj);
        }
        
        auto** ptr = &gcHead_;
        while(*ptr) {
            if(!(*ptr)->mark_) {
                auto* garbage = *ptr;
                *ptr = garbage->next_;
                deallocate(garbage);
            } else {
                (*ptr)->mark_ = false;
                ptr = &(*ptr)->next_;
            }
        }
        
        nextGC_ = allocated_ * 2;
    }
    
    void Context::markObject(const Object* object) const {
        if(object->mark_) return;
        
        allocated_ += sizeof(Object);
        object->mark_ = true;
        for(UInt16 i = 0; i < object->size_; ++i) {
            object->property(i)
                | match<Object*>([this](auto obj) { markObject(obj); })
                | match<Array<Object*>>([this](const auto& array){
                    for(auto* obj: array) markObject(obj);
                });
        }
    }
}
