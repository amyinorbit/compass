//===--------------------------------------------------------------------------------------------===
// infer.hpp - Inference engine (basically, the semantic analyzer of Compass)
//
// Created by Amy Parent <amy@amyparent.com>
// Copyright (c) 2019 Amy Parent
// Licensed under the MIT License
// =^•.•^=
//===--------------------------------------------------------------------------------------------===
#pragma once
#include <compass/runtime2/primitives.hpp>
#include <apfun/string.hpp>
#include <apfun/maybe.hpp>
#include <unordered_map>
#include <string>
#include <memory>

namespace amyinorbit::compass {
    using cbuf = std::string;
    
    namespace inferred {
        
        enum class VK { boolean, real, text, thing, collection };
        
        struct Link {
            string direction;
            cbuf from, to;
        };
        
        
    };
    
    class Infer {
    public:
        
        void declareDirection(const string& dir);
        void declareDirection(const string& dir, const string& opposite);
        
    private:
        template <typename T> using ref = std::unique_ptr<T>;
        
        // The object that is currently being talked about.
        // inferred::Object* current_;
        
        // All known things.
        map<string, maybe<string>> directions_;
    };
}
