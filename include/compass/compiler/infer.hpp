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
                std::cout << "-" << id << ": " << *obj << "\n";
            }
        }

    private:

        bool error(bool expr, const string& error) {
            if(expr) {
                driver_.diagnostic(Diagnostic::error(error));
            }
            return expr;
        }

        Object* get_or_create(const string& name);

        Driver& driver_;
        Garbage gc_;
        maybe<Ref> ref_;
        Object* thingKind_ = nullptr;
        Object* roomKind_ = nullptr;
        Object* directionKind_ = nullptr;

        map<string, Object*> world_;
        map<string, Object*> prototypes_;
        map<string, Object*> directions_;

        // This is probably not the best way to represent properties:
        //  1. it doesn't ensure that each property value has a globally unique name
        //  2. it makes it extremely inefficient to recover a property from one of its values
        //
        //      using EnumValues std::vector<string>;
        //      map<string, EnumValues> properties_;
        //
        // Instead, this feels like a better idea. Not it still requires keeping track of existing
        // properties for referring to them in text.
        map<string, string> values_; // property value -> property name
        set<string> properties_; // property names
    };
}
