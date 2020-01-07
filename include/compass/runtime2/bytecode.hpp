//===--------------------------------------------------------------------------------------------===
// bytecode.hpp - Bytecode enumerator for Compass 2.0
//
// Created by Amy Parent <amy@amyparent.com>
// Copyright (c) 2020 Amy Parent
// Licensed under the MIT License
// =^•.•^=
//===--------------------------------------------------------------------------------------------===
#pragma once

namespace Compass::rt2 {

    #define OPCODE(name, _, __) name,
    enum class Bytecode {
    #include "bytecode.x.hpp"
    };
    #undef OPCODE

}
