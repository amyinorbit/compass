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
            for(const auto& [id, obj]: world_) {
                std::cout << "-" << id << ":\n";
                for(const auto& [field, _]: obj->fields()) {
                    std::cout << "  - " << field << "\n";
                }
            }
        }

    private:

        bool error(bool expr, const string& error) {
            if(expr) {
                driver_.diagnostic(Diagnostic::error(error));
            }
            return expr;
        }

        Driver& driver_;
        // Garbage gc_;
        maybe<Ref> ref_;
        type::TypeDB world_;
        map<string, string> values_; // property value -> property name
        set<string> properties_; // property names
    };
}
