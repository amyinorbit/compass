//===--------------------------------------------------------------------------------------------===
// function.cpp - Implementation of
// This source is part of the Compass Engine
//
// Created on 2019-03-05 by Amy Parent <amy@amyparent.com>
// Copyright (c) 2019 Amy Parent
// Licensed under the MIT License
// =^•.•^=
//===--------------------------------------------------------------------------------------------===
#include <compass/type/function.hpp>

namespace Compass::Type {
    /*
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
    */
    Function::Function(ForeignTag tag, ForeignImpl fn) : foreign_(fn) {

    }

    Function::Function::Function(BytecodeTag tag, u64 size) : bytecode_() {

    }

    Function::~Function() {
        switch(kind_) {
            case Kind::Foreign:
            foreign_.~ForeignImpl();
            break;

            case Kind::Bytecode:
            bytecode_.~vector();
            break;
        }
    }
}
