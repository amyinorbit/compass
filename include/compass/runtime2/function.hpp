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
#include <functional>
#include <compass/runtime2/primitives.hpp>

namespace Compass::rt2 {
    class Context;
    class Object;
    static constexpr struct ForeignTag {} foreign;
    static constexpr struct BytecodeTag {} bytecode;

    class Function : NonCopyable, NonMovable {
    public:

        using ForeignImpl = std::function<void(Object*, Context&)>;

        enum class Kind { Foreign, Bytecode };

        Function(ForeignTag tag, ForeignImpl fn);
        Function(BytecodeTag tag, u64 size);

        ~Function();

        Kind kind() const { return kind_; }

    private:
        Kind kind_;
        union {
            std::vector<u16> bytecode_;
            ForeignImpl      foreign_;
        };
    };
}
