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
#include <compass/core/functional.hpp>
#include <compass/language/grammar.hpp>

namespace Compass::Language {
    
    class Driver {
    public:
        virtual ~Driver() {}
        
        virtual const Grammar& grammar() const = 0;
        virtual void error(const std::string& message) = 0;
        virtual bool isFailed() const = 0;
    };
}
