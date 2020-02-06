//===--------------------------------------------------------------------------------------------===
// driver.hpp - Interface used for parser drivers
// This source is part of the Compass Engine
//
// Created on 2019-02-22 by Amy Parent <amy@amyparent.com>
// Copyright (c) 2019 Amy Parent
// Licensed under the MIT License
// =^•.•^=
//===--------------------------------------------------------------------------------------------===
#pragma once
#include <compass/language/diagnostics.hpp>
#include <compass/language/grammar.hpp>

namespace amyinorbit::compass {

    class Driver {
    public:
        virtual ~Driver() {}

        virtual const Grammar& grammar() const = 0;
        virtual void diagnostic(const Diagnostic& diag) = 0;
        virtual bool is_failed() const = 0;
    };
}
