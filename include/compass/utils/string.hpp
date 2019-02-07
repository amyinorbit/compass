//===--------------------------------------------------------------------------------------------===
// string.hpp - String utilities
// This source is part of the Compass Engine
//
// Created on 2019-02-07 by Amy Parent <amy@amyparent.com>
// Copyright (c) 2019 Amy Parent
// Licensed under the MIT License
// =^•.•^=
//===--------------------------------------------------------------------------------------------===
#pragma once
#include <string>

namespace Compass {
    namespace String {
        inline std::string toLower(const std::string& str) {
            std::string lower = str;
            for(auto& c: lower) c = ::tolower(c);
            return lower;
        }
    }
}
