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

    void Compiler::diagnose(std::ostream &out) const {
        for(const auto& d: diagnostics_) {
            switch(d.level()) {
            case Diagnostic::Level::Progress:
                out << "[" << d.title() << "]\n";
                break;
            case Diagnostic::Level::Info:
                out << " * " << d.title() << "\n";
                break;
            case Diagnostic::Level::Error:
                out << "[ERROR] " << d.title() << "\n";
                break;
            }
        }
    }
}
