//===--------------------------------------------------------------------------------------------===
// sema.hpp - Compass semantic checker - checks both CNL and CASM
//
// Created by Amy Parent <amy@amyparent.com>
// Copyright (c) 2019 Amy Parent
// Licensed under the MIT License
// =^•.•^=
//===--------------------------------------------------------------------------------------------===
#pragma once
#include <compass/types.hpp>
#include <compass/compiler/type.hpp>
#include <compass/language/driver.hpp>
#include <apfun/maybe.hpp>
#include <memory>

namespace amyinorbit::compass::sema {

    struct Link {
        string direction;
        string from, to;
    };

    class Sema {
    public:
        using World = map<string, Value>;

        Sema(Driver& driver) : driver_(driver) {}

        bool exists(const string& name) const {
            return world_.count(name) != 0;
        }

        Object* object(const string& name);
        Object* kind(const string& name);
        maybe<string> property_of(const string& value) const;

        Object* create_object(const Object* proto, const string& name);
        Object* create_kind(const Object* proto, const string& name);
        void create_property(const string& name);
        void create_value(const string& property, const string& value);
        bool ensure_not_exists(const string& name);

        void write(std::ostream &out);

        const World& world() const { return world_; }

    private:

        bool error(bool expr, const string& error) {
            if(expr) {
                driver_.diagnostic(Diagnostic::error(error));
            }
            return expr;
        }

        Driver& driver_;
        set<string> properties_;
        map<string, string> values_;
        map<string, std::unique_ptr<Object>> kinds_;
        map<string, std::unique_ptr<Object>> objects_;
        map<string, Value> world_;
        vector<Link> links_;
    };
}
