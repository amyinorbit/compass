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
#include <compass/runtime2/buffer.hpp>

namespace Compass::rt2 {

    class Machine : NonCopyable, NonMovable {
    public:
        Machine(const Context& ctx);
        bool run(const string& function = "<script>");

    private:

        struct Frame {
            string name;
            const u16* ip;
            Value* base;
        };

        void openFrame(const Function& fn);
        void closeFrame();

        u16 read() const;
        Bytecode readCode() const;

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


        // MARK: - Story tracking

        Object* location_ = nullptr;
        Object* lookat_ = nullptr;
        const Context& ctx_;

        // MARK: - Garbage collection

        Object* gcHead_ = nullptr;
        u64 nextGC_ = 0;
        u64 allocated_ = 0;

        map<string, Object*> prototypes_;

        // MARK: - Runtime VM

        mutable const u16* ip_ = nullptr;
        buffer<Value> stack_;
        buffer<Frame> callStack_;
    };
}
