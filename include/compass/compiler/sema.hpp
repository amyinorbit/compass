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
    class Sema {
    public:

        Sema() {}
        Object* object(const string& name);
        Object* create_object(const Object* proto, const string& name);
        void write(std::ostream &out) const;
        void print_index() const;

    private:
        using Ref = std::unique_ptr<Object>;
        map<string, Ref> objects_;
    };
}
