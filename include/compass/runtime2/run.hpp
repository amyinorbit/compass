//===--------------------------------------------------------------------------------------------===
// run.hpp - runtime2's virtual machine
//
// Created by Amy Parent <amy@amyparent.com>
// Copyright (c) 2020 Amy Parent
// Licensed under the MIT License
// =^•.•^=
//===--------------------------------------------------------------------------------------------===
#pragma once
#include <compass/runtime2/context.hpp>
#include <compass/runtime2/primitives.hpp>

namespace Compass::rt2 {

    class Run {
    public:
        Run(const Context& ctx);

        bool run(const string& function = "<script>");

    private:

        struct Frame {
            Function* fn;
            const u16* ip;
            Value* sp;
        };

        #define OPCODE(name, _, __) name,
        enum class Code {
        #include "bytecode.hpp"
        };
        #undef OPCODE

        u16 read() const;
        Code readCode() const;

        Value pop() { auto v = stack_.back(); stack_.pop_back(); return v; }
        template <typename T>
        void push(const T& value) { stack_.push_back(value); }

        template <typename T>
        T pop() {
            Value v = stack_.back();
            stack_.pop_back();
            return v.as<T>();
        }

        template <typename T>
        const T& peek() const { return std::get<T>(stack_.back()); }
        template <typename T>
        T& peek() { return stack_.back().as<T>(); }
        Value peek() const { return stack_.back(); }
        void swap() { std::iter_swap(stack_.end() - 1, stack_.end() - 2); }

        template <typename T>
        const T& constant() const { return ctx_.constants[read()].as<T>(); }
        template <typename T>
        const T& constant(u16 idx) const { return ctx_.constants[idx].as<T>(); }
        const Value& constant() const { return ctx_.constants[read()]; }

        void collect();
        Object* clone(const Object* other);
        Object* allocate(const string& kind, const Object* prototype = nullptr);
        Object* allocate(const string& kind, const string& prototype);
        const Object* prototype(const string& kind) const { return prototypes_.at(kind); }

        Object* location_ = nullptr;
        Object* lookat_ = nullptr;
        const Context& ctx_;

        Object* gcHead_ = nullptr;
        u64 nextGC_ = 0;
        u64 allocated_ = 0;

        map<string, Object*> prototypes_;
        vector<Value> stack_;
        mutable const u16* ip_;
    };
}
