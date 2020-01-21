//===--------------------------------------------------------------------------------------------===
// vm.hpp - Compass runtime virtual machine
//
// Created by Amy Parent <amy@amyparent.com>
// Copyright (c) 2019 Amy Parent
// Licensed under the MIT License
// =^•.•^=
//===--------------------------------------------------------------------------------------------===
#pragma once
#include <compass/runtime2/bytecode.hpp>
#include <compass/runtime2/memory.hpp>

namespace amyinorbit::compass {

    class Stack {
    public:
        using size_type = Memory::size_type;
        Stack(size_type capacity) : memory_(capacity), top_(0) {}

        template <typename T, std::enable_if_t<sizeof(T) <= 4>* = nullptr>
        void push(const T& value) {
            memory_.write(top_, value);
            top_ += 4;
        }

        template <typename T, std::enable_if_t<sizeof(T) <= 4>* = nullptr>
        T pop() {
            top_ -= 4;
            return memory_.read<T>(top_);
        }

        template <typename T, std::enable_if_t<sizeof(T) <= 4>* = nullptr>
        const T& peek() const {
            return memory_.read<T>(top_ - 1);
        }
    private:
        Memory memory_;
        size_type top_;
    };

    /*
    Story files contain the following:

    ============ STORY HEADER ============
     compass version number
     story file version
     constant memory size
     globals memory size

     functions offset
     constants offset
     start pointer
    ============   CONSTANTS   ============

    */

    class VM {
    public:
        static constexpr u32 kb = 1024;
        static constexpr u32 mb = 1024 * kb;

        VM();

    private:

        Stack stack_{10 * mb};
        Memory constants_{10 * mb};
        Memory heap_{10 * mb};
    };

}
