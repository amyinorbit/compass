//===--------------------------------------------------------------------------------------------===
// compiler.cpp - Compass compiler driver implementation
//
// Created by Amy Parent <amy@amyparent.com>
// Copyright (c) 2019 Amy Parent
// Licensed under the MIT License
// =^•.•^=
//===--------------------------------------------------------------------------------------------===
#include <compass/compiler/compiler.hpp>

namespace amyinorbit::compass {

    void Compiler::diagnostic(const Diagnostic &diag) {
        if(diag.level() == Diagnostic::Level::Error) {
            errorCount_ += 1;
        }
        diagnostics_.push_back(diag);
    }
}
