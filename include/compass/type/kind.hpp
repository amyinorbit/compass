//===--------------------------------------------------------------------------------------------===
// kind.hpp - Internal representation of a COmpass object class
// This source is part of the Compass Engine
//
// Created on 2019-03-02 by Amy Parent <amy@amyparent.com>
// Copyright (c) 2019 Amy Parent
// Licensed under the MIT License
// =^•.•^=
//===--------------------------------------------------------------------------------------------===
#pragma once
#include <vector>
#include <compass/type/primitives.hpp>

namespace Compass::Type {
    
    class Kind {
    public:
        struct Field {
            String  name;
            // TODO: add a type/kind tag
        };
        
        Kind(const String& name, Kind* super = nullptr)
            : super_(super), name_(name) {}
        
        const Kind* super() const { return super_; }
        const String& name() const { return name_; }
        
        UInt16 size() const;
        void addField(const String& name);
        optional<UInt16> field(const String& name) const;
        
    private:
        const Kind*     super_;
        String          name_;
        // TODO: change that to some small-array optimisation probably
        Array<Field>    fields_;
    };
}
