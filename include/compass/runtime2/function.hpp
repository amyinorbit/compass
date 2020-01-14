//===--------------------------------------------------------------------------------------------===
// function.hpp - Compass's bytecode/foreign function interface
// This source is part of the Compass Engine
//
// Created on 2019-03-04 by Amy Parent <amy@amyparent.com>
// Copyright (c) 2019 Amy Parent
// Licensed under the MIT License
// =^•.•^=
//===--------------------------------------------------------------------------------------------===
#pragma once
#include <compass/types.hpp>
#include <compass/runtime2/bytecode.hpp>

namespace amyinorbit::compass {
    class Machine;
    class Object;


    class Function : NonCopyable, NonMovable {
    public:
        const u16* ip() const { return &bytecode_.front(); }

        void emit(Bytecode inst);
        void emit(Bytecode inst, u16 constant);

        u16 emitJump(Bytecode inst);
        void patchJump(u16 id);

    private:
        std::vector<u16> bytecode_;
    };
}
