//===--------------------------------------------------------------------------------------------===
// function.cpp - Implementation of
// This source is part of the Compass Engine
//
// Created on 2019-03-05 by Amy Parent <amy@amyparent.com>
// Copyright (c) 2019 Amy Parent
// Licensed under the MIT License
// =^•.•^=
//===--------------------------------------------------------------------------------------------===
#include <compass/runtime2/function.hpp>
#include <cassert>

namespace Compass::rt2 {

    /*
        void emit(Bytecode inst);
        void emit(Bytecode inst, u16 constant);

        u16 emitJump(Bytecode inst);
        void patchJump(u16 id);
    */
    void Function::emit(Bytecode inst) {
        bytecode_.push_back(static_cast<u16>(inst));
    }

    void Function::emit(Bytecode inst, u16 constant) {
        emit(inst);
        bytecode_.push_back(constant);
    }

    u16 Function::emitJump(Bytecode inst) {
        emit(inst);
        u16 loc = bytecode_.size();
        bytecode_.push_back(0xcafe);
        return loc;
    }

    void Function::patchJump(u16 id) {
        assert(id < bytecode_.size());
        i32 loc = bytecode_.size();
        bytecode[id] = loc - id;
    }
}
