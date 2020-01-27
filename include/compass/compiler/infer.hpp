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
#include <compass/compiler/sema.hpp>
#include <apfun/string.hpp>
#include <apfun/maybe.hpp>
#include <string>
#include <memory>

namespace amyinorbit::compass {

    struct Link {
        string direction;
        string from, to;
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
                std::cout << "@" << id << ": " << obj;
                std::cout << "\n";
            }
        }

    private:
        maybe<sema::Value> anything(const string& name) const {
            if(!world_.count(name)) return nothing();
            return world_.at(name);
        }

        bool exists(const string& name) const {
            return world_.count(name) != 0;
        }

        sema::Object* object(const string& name);
        sema::Object* kind(const string& name);
        maybe<string> property_of(const string& value) const;

        sema::Object* create_object(const sema::Object* proto, const string& name);
        sema::Object* create_kind(const sema::Object* proto, const string& name);
        void create_property(const string& name);

        bool ensure_not_exists(const string& name);

        bool error(bool expr, const string& error) {
            if(expr) {
                driver_.diagnostic(Diagnostic::error(error));
            }
            return expr;
        }

        Driver& driver_;
        maybe<Ref> ref_;

        set<string> properties_;
        map<string, string> values_;
        map<string, std::unique_ptr<sema::Object>> kinds_;
        map<string, std::unique_ptr<sema::Object>> objects_;

        map<string, sema::Value> world_;
        vector<Link> links_;
    };
}
