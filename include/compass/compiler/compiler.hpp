//===--------------------------------------------------------------------------------------------===
// compiler.hpp - Compass's compiler driver
//
// Created by Amy Parent <amy@amyparent.com>
// Copyright (c) 2019 Amy Parent
// Licensed under the MIT License
// =^•.•^=
//===--------------------------------------------------------------------------------------------===
#pragma once
#include <compass/language/driver.hpp>
#include <compass/language/grammar.hpp>
#include <vector>

namespace amyinorbit::compass {
    /*
    class Driver {
    public:
        virtual ~Driver() {}

        virtual const Grammar& grammar() const = 0;
        virtual void diagnostic(const Diagnostic& diag) = 0;
        virtual bool isFailed() const = 0;
    };
    */
    class Compiler: public Driver {
    public:
        virtual ~Compiler() {}

        virtual const Grammar& grammar() const override { return grammar_; }
        virtual void diagnostic(const Diagnostic& diag) override;
        virtual bool isFailed() const override { return errorCount_ > 0; }
    private:
        BasicEnglish grammar_;
        int errorCount_ = 0;
        std::vector<Diagnostic> diagnostics_;
    };
}
