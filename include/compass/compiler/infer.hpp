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
#include <compass/compiler/type.hpp>
#include <compass/compiler/sema.hpp>
#include <apfun/string.hpp>
#include <apfun/maybe.hpp>
#include <string>
#include <memory>

namespace amyinorbit::compass {

    class InferEngine {
    public:
        InferEngine(Driver& driver, sema::Sema&);

        struct Ref {
            string obj;
            maybe<string> field = nothing();
        };

        void select(const string& what);
        void select(const string& what, const string& field);

        void new_kind(const string& prototype);
        void new_property(const string& prototype);
        void new_property();
        void set_kind(const string& kind);

        void contained(const string& how, const string& in_what);
        void link_to(const string& direction, const string& place);

        void set_property(const string& prop);
        void declare_property(const string& property, const string& value);

        void dump() const {
            std::cout << "world: ";
            for(const auto& [id, obj]: sema_.world()) {
                std::cout << id << ", ";
            }
            std::cout << "\n";
        }

    private:


        bool error(bool expr, const string& error) {
            if(expr) {
                driver_.diagnostic(Diagnostic::error(error));
            }
            return expr;
        }

        Driver& driver_;
        sema::Sema& sema_;
        maybe<Ref> ref_;
    };
}
