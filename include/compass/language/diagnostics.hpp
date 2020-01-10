//===--------------------------------------------------------------------------------------------===
// diagnostics.hpp - Interface for diagnostics forwarding/message passing in the compiler
// This source is part of the Compass Engine
//
// Created on 2019-02-25 by Amy Parent <amy@amyparent.com>
// Copyright (c) 2019 Amy Parent
// Licensed under the MIT License
// =^•.•^=
//===--------------------------------------------------------------------------------------------===
#pragma once
#include <cstdint>
#include <string>
#include <apfun/maybe.hpp>
#include <apfun/string.hpp>

namespace amyinorbit::compass {

    class Diagnostic {
    public:
        using ID = std::uint32_t;
        enum class Level { Progress, Info, Error }; // TODO: probably add support for other levels

        //Diagnostic(ID id); // TODO: add support for pre-compiled list of diagnostics + IDS
        Diagnostic(Level level, const string& title)
            : level_(level), title_(title), message_(nothing()) {}
        Diagnostic(Level level, const string& title, const string& message)
            : level_(level), title_(title), message_(message) {}

        static Diagnostic error(const string& message) {
            return Diagnostic(Level::Error, message);
        }

        Level level() const { return level_; }
        const string& title() const { return title_; }
        const maybe<string>& message() const { return message_; }
    private:
        Level                   level_;
        string                  title_;
        maybe<string>           message_;
    };

    using DiagnosticsConsumer = std::function<void(const Diagnostic&)>;


}
