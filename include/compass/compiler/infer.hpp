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
#include <compass/compiler/world.hpp>
#include <boost/bimap.hpp>
#include <apfun/string.hpp>
#include <apfun/maybe.hpp>
#include <algorithm>
#include <string>
#include <memory>

namespace amyinorbit::compass {

    using boost::bimap;

    class InferEngine {
    public:
        InferEngine(Driver& driver, sema::Sema&);

        struct Ref {
            string obj;
            maybe<string> field = nothing();
        };

        void select(const Ref& what) { ref_ = what; }
        void select(const string& what);
        void select(const string& what, const string& field);

        void new_kind(const string& prototype);
        void new_property(const string& prototype);
        void new_property_value(const string& property);
        void new_property();
        void set_kind(const string& kind);

        bool is_a(const string& what);

        void contained(const string& how, const string& in_what);
        void link_to(const string& direction, const string& place);

        void set_property(const string& prop);
        void declare_property(const string& property, const string& value);

        Value::Type type_of(const string& name) const;

        const string& singular(const string& plural) const {
            auto low = plural.lowercased();
            return world_.plurals.right.count(low) ? world_.plurals.right.at(low) : plural;
        }

    private:

        bool error(bool expr, const string& msg) {
            if(expr) { error(msg); }
            return expr;
        }

        void error(const string& error) const {
            driver_.diagnostic(Diagnostic::error(error));
        }

        void set_plural(const string& singular, const string& plural);

        /// MAARK: - Data Access

        bool ensure_exists(const string& name) const {
            if(world_.exists(name)) return true;
            error("There is nothing named '" + name + "'");
            return false;
        }

        bool ensure_not_exists(const string& name) const {
            if(!world_.exists(name)) return true;
            error("There is already something else named '" + name + "'");
            return false;
        }

        // MARK: - Data

        Driver& driver_;
        World world_;
        sema::Sema& sema_;

        maybe<Ref> ref_ = nothing();
    };
}
