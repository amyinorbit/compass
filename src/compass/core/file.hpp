//===--------------------------------------------------------------------------------------------===
// file.hpp - Private definititions used by the binary file writer and reader.
// This source is part of the Compass Engine
//
// Created on 2019-02-23 by Amy Parent <amy@amyparent.com>
// Copyright (c) 2019 Amy Parent
// Licensed under the MIT License
// =^•.•^=
//===--------------------------------------------------------------------------------------------===
#pragma once
#include <cstdint>

namespace Compass {
    
    enum class Tag: std::uint8_t {
        
        U       = 0b00010000,
        U8      = 0b00010001,
        U16     = 0b00010010,
        U32     = 0b00010100,
        U64     = 0b00011000,
        
        Str     = 0b00100000,
        StrID   = 0b00100001,
        StrData = 0b00100010,
        
        Entity  = 0b01000000,
        Thing   = 0b01000001,
        Place   = 0b01000010,
    };
    
}
