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
#include <functional>
#include <string>
#include <optional>

namespace amyinorbit::compass {
    
    class Diagnostic {
    public:
        using ID = std::uint32_t;
        enum Level { Progress, Info, Error }; // TODO: probably add support for other levels
        
        //Diagnostic(ID id); // TODO: add support for pre-compiled list of diagnostics + IDS
        Diagnostic(Level level, const std::string& title)
            : level_(level), title_(title), message_({}) {}
        Diagnostic(Level level, const std::string& title, const std::string& message)
            : level_(level), title_(title), message_(message) {}
        
        Level level() const { return level_; }
        const std::string& title() const { return title_; }
        const std::optional<std::string>& message() const { return message_; }
    private:
        Level                   level_;
        std::string             title_;
        std::optional<std::string> message_;
    };
    
    using DiagnosticsConsumer = std::function<void(const Diagnostic&)>;
    
    
}
