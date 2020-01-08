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
#define OPCODE(name, _, __)
#endif

OPCODE(halt,            0,   0)

OPCODE(load_true,       0,  +1)
OPCODE(load_false,      0,  +1)
OPCODE(load_const,      1,  +1)
OPCODE(load_prop,       1,  +1)
OPCODE(load_var,        1,  +1)

OPCODE(store_prop,      1,  -2)
OPCODE(store_var,       1,  -1)

OPCODE(pop,             0,  -1)
OPCODE(swap,            0,   0)
OPCODE(dup,             0,  +1)

OPCODE(io_style,        1,   0)
OPCODE(io_read,         0,  +1)
OPCODE(io_write,        0,  -1)
OPCODE(parse,           0,  +1)

OPCODE(add,             0,  -1)
OPCODE(sub,             0,  -1)
OPCODE(mul,             0,  -1)
OPCODE(div,             0,  -1)

OPCODE(comp,            0,  -1)
OPCODE(jump_if,         1,   0)
OPCODE(rjump_if,        1,   0)
OPCODE(jump,            1,   0)
OPCODE(rjump,           1,   0)

OPCODE(link,            1,   0)
OPCODE(clone,           1,  +1)

OPCODE(call,            0,  +2)
OPCODE(ret,             0,   0)

#undef OPCODE
