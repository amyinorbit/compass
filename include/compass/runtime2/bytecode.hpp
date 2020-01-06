//===--------------------------------------------------------------------------------------------===
// bytecode.hpp - Byte code operations available to the compass runtime
//
// Created by Amy Parent <amy@amyparent.com>
// Copyright (c) 2020 Amy Parent
// Licensed under the MIT License
// =^•.•^=
//===--------------------------------------------------------------------------------------------===
#pragma once

// We use the x-macro pattern here for convenience

#ifndef OPCODE
#define OPCODE(name, _, __) name,
#endif

OPCODE(halt,            0,   0)

OPCODE(push_true,       0,  +1)
OPCODE(push_false,      0,  +1)
OPCODE(push_const,      1,  +1)
OPCODE(push_prop,       1,  +1)
OPCODE(push_current,    0,  +1)

OPCODE(store_prop,      1,  -2)
OPCODE(store_current    0,  -1)

OPCODE(pop,             0,  -1)
OPCODE(swap,            0,   0)
OPCODE(dup,             0,  +1)

OPCODE(comp,            0,  -1)
OPCODE(jump_if,         1,   0)
OPCODE(rjump_if,        1,   0)
OPCODE(jump,            1,   0)
OPCODE(rjump,           1,   0)

OPCODE(go_link,         1,   0)
OPCODE(look,            1,   0)

OPCODE(verb,            0,   2)

#undef OPCODE