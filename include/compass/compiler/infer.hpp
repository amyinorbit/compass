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
#include <algorithm>
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

        void select(const Ref& what) { ref_ = what; }
        void select(const string& what);
        void select(const string& what, const string& field);

        void new_kind(const string& prototype);
        void new_property(const string& prototype);
        void new_property_value(const string& property);
        void new_property();
        void set_kind(const string& kind);

        void is_a(const string& what);

        void contained(const string& how, const string& in_what);
        void link_to(const string& direction, const string& place);

        void set_property(const string& prop);
        void declare_property(const string& property, const string& value);

        const string& singular(const string& plural) const {
            auto it = singular_it(plural);
            return it != plurals_.end() ? it->first : plural;
        }

        void dump() const {
            std::cout << "world: ";
            for(const auto& [id, obj]: sema_.world()) {
                std::cout << id << ", ";
            }
            std::cout << "\n";
        }

        using Two = std::pair<string, string>;
    private:

        bool error(bool expr, const string& error) {
            if(expr) {
                driver_.diagnostic(Diagnostic::error(error));
            }
            return expr;
        }

        void error(const string& error) {
            driver_.diagnostic(Diagnostic::error(error));
        }

        vector<Two>::const_iterator plural_it(const string& singular) const {
            return std::find_if(plurals_.begin(), plurals_.end(), [&](const auto& p) {
                return p.first == singular;
            });
        }

        vector<Two>::iterator plural_it(const string& singular) {
            return std::find_if(plurals_.begin(), plurals_.end(), [&](const auto& p) {
                return p.first == singular;
            });
        }

        vector<Two>::const_iterator singular_it(const string& plural) const {
            return std::find_if(plurals_.begin(), plurals_.end(), [&](const auto& p) {
                return p.second == plural;
            });
        }

        vector<Two>::iterator singular_it(const string& plural) {
            return std::find_if(plurals_.begin(), plurals_.end(), [&](const auto& p) {
                return p.second == plural;
            });
        }

        void set_plural(const string& singular, const string& plural);

        Driver& driver_;
        sema::Sema& sema_;

        vector<Two> plurals_;
        maybe<Ref> ref_ = nothing();
    };
}
