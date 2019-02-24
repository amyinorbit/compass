//===--------------------------------------------------------------------------------------------===
// loader.hpp - Private interface for the loader
// This source is part of the Compass Engine
//
// Created on 2019-02-23 by Amy Parent <amy@amyparent.com>
// Copyright (c) 2019 Amy Parent
// Licensed under the MIT License
// =^•.•^=
//===--------------------------------------------------------------------------------------------===
#include <cstring>
#include <iostream>
#include <compass/core/file.hpp>
#include <compass/core/functional.hpp>
#include "file.hpp"

namespace Compass {
    
    class Loader {
    public:
        
        Loader(std::istream& in) : in_(in) {}
        result<Story> load();
        
    private:
        
        void recSignature();
        void recMetadata();
        void recStringPool();
        void recDirections();
        void recVerbs();
        
        void recEntities();
        result<Entity> recEntity();
        
        result<Relation> recEntityContainer();
        result<Action> recEntityAction();
        bool recEntityThing(Entity& e);
        result<Link> recEntityLink();
        
        optional<std::uint8_t> readU8();
        optional<std::uint16_t> readU16();
        optional<std::uint32_t> readU32();
        optional<std::uint64_t> readU64();
        optional<std::string> readString();
        optional<StringID> readStringID();
        
        template <typename T>
        optional<T> readEnum() {
            if(failed()) return {};
            char v;
            if(!in_.get(v)) return {};
            return static_cast<T>(v);
        }
        
        template <typename T>
        bool have(T tag) {
            if(failed()) return false;
            char test = in_.peek();
            return static_cast<T>(test) == tag;
        }
    
        template <typename T>
        bool match(T tag) {
            if(failed()) return false;
            char test;
            if(!in_.get(test)) return false;
            return static_cast<T>(test) == tag;
        }
        
        void error(const std::string& msg) { error_ = msg; }
        bool failed() const { return error_.has_value(); }
        
        Story                   story_;
        std::istream&           in_;
        optional<std::string>   error_;
    };
}
