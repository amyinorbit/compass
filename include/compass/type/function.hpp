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
#include <compass/type/primitive>

namespace Compass::Type {
    class Context;
    class Object;
    
    class Function {
    public:
        
        static constexpr struct ForeignTag {} foreign;
        static constexpr struct BytecodeTag {} bytecode;
        
        using ForeignImpl = std::function<void(Object*, Context&)>;
        
        enum class Kind { Foreign, Bytecode };
        
        Function(ForeignTag tag, ForeignImpl fn);
        Function(BytecodeTag tag, UInt64 size);
        
        Function(const Function&) = delete;
        Function(Function&&) = delete;
        Function& operator=(const Function&) = delete;
        Function& operator=(Function&&) = delete;
        
        ~Function();
        
        Kind kind() const { return kind_; }
        
    private:
        Kind kind_;
        enum {
            std::vector<UInt16> bytecode_;
            ForeignImpl         foreign_;
        };
    };
}
