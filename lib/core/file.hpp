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
    };
    
    enum class Section: std::uint8_t {
        Metadata    = 0x70,
        StringPool  = 0x71,
        Directions  = 0x72,
        Verbs       = 0x73,
        Entities    = 0x74,
        Place       = 0x75,
        Thing       = 0x76,
        Container   = 0x77,
        Things      = 0x78,
        Links       = 0x79,
        Actions     = 0x7a,
        
        Start       = 0x7e,
        End         = 0x7f,
    };
    
    enum class Marker: std::uint8_t {
        EntityKind  = 0x80,
        RelationKind= 0x81,
        ActionKind  = 0x82,
        VerbKind    = 0x83,
    };
    
    enum class Metadata: std::uint8_t {
        Title       = 0x01,
        Author      = 0x02,
        ComVersion  = 0x03,
        Date        = 0x04,
    };
}
