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

namespace amyinorbit::compass {
    using cbuf = std::string;

    class Diagnostic {
    public:
        using ID = std::uint32_t;
        enum class Level { Progress, Info, Error }; // TODO: probably add support for other levels

        //Diagnostic(ID id); // TODO: add support for pre-compiled list of diagnostics + IDS
        Diagnostic(Level level, const cbuf& title)
            : level_(level), title_(title), message_(nothing()) {}
        Diagnostic(Level level, const cbuf& title, const cbuf& message)
            : level_(level), title_(title), message_(message) {}

        static Diagnostic error(const cbuf& message) {
            return Diagnostic(Level::Error, message);
        }

        Level level() const { return level_; }
        const cbuf& title() const { return title_; }
        const maybe<cbuf>& message() const { return message_; }
    private:
        Level                   level_;
        cbuf                  title_;
        maybe<cbuf>           message_;
    };

    using DiagnosticsConsumer = std::function<void(const Diagnostic&)>;


}
