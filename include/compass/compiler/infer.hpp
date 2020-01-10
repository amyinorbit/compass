//===--------------------------------------------------------------------------------------------===
// infer.hpp - Inference engine (basically, the semantic analyzer of Compass)
//
// Created by Amy Parent <amy@amyparent.com>
// Copyright (c) 2019 Amy Parent
// Licensed under the MIT License
// =^•.•^=
//===--------------------------------------------------------------------------------------------===
#pragma once
#include <compass/language/driver.hpp>
#include <compass/runtime2/garbage.hpp>
#include <apfun/string.hpp>
#include <apfun/maybe.hpp>
#include <string>

namespace amyinorbit::compass {
    using cbuf = std::string;

    namespace inferred {
        struct Link {
            string direction;
            string from, to;
        };
    };

    class InferEngine {
    public:
        InferEngine(Driver& driver);

        void declareDirection(const string& dir);
        void declareDirection(const string& dir, const string& opposite);

        void refer(const string& name);

        void kind(const string& kind);
        void adjectives(const std::vector<string>& adj);

        void describe(const string& desc);
        void relation(const string& rel, const string& other);
        void location(const string& direction, const string& other);
        
        
        void dump() const {
            std::cout << *current_ << "\n";
            std::cout << *thingKind_ << "\n";
            std::cout << *roomKind_ << "\n";
        }

    private:
        Driver& driver_;

        bool error(bool expr, const string& error) {
            if(expr) {
                driver_.diagnostic(Diagnostic::error(error));
            }
            return expr;
        }

        Garbage gc_;
        Object* current_ = nullptr;
        Object* thingKind_ = nullptr;
        Object* roomKind_ = nullptr;

        map<string, Object*> world_;
        map<string, Object*> prototypes_;
        map<string, maybe<string>> directions_;
    };
}
