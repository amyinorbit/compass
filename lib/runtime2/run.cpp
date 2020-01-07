//===--------------------------------------------------------------------------------------------===
// context.cpp - Implementation of base Compass Objects
// This source is part of the Compass Engine
//
// Created on 2019-03-03 by Amy Parent <amy@amyparent.com>
// Copyright (c) 2019 Amy Parent
// Licensed under the MIT License
// =^•.•^=
//===--------------------------------------------------------------------------------------------===
#include <cassert>
#include <compass/runtime2/run.hpp>
#include <compass/runtime2/object.hpp>
#include <compass/runtime2/bytecode.hpp>

namespace Compass::rt2 {


    Run::Run(const Context& ctx) : ctx_(ctx) {
        stack_ = new Value[512];
        stackTop_ = stack_;
        for(const auto& p: ctx.prototypes) {
            prototypes_[p.kind()] = clone(&p);
        }
    }

    Run::~Run() {
        delete [] stack_;
    }

    u16 Run::read() const {
        return *(ip_++);
    }


    void Run::openFrame(const Function& fn) {
        Frame f;
        f.ip = ip_;
        f.base = stackTop_;
        callStack_.push_back(f);
        ip_ = fn.ip();
    }

    void Run::closeFrame() {
        assert(callStack_.size());
        const auto& f = callStack_.back();
        callStack_.pop_back();
        stackTop_ = f.base;
        ip_ = f.ip;
    }

    Bytecode Run::readCode() const {
        return static_cast<Bytecode>(read());
    }

    bool Run::run(const string& signature) {
        if(!ctx_.functions.count(signature)) return false;
        const auto& fn = ctx_.functions.at(signature);

        ip_ = fn.ip();
        Bytecode inst = Bytecode::halt;

        while((inst = readCode()) != Bytecode::halt) {
            switch (inst) {
                case Bytecode::halt: return true;

                case Bytecode::push_true: push<bool>(true); break;
                case Bytecode::push_false: push<bool>(false); break;
                case Bytecode::push_const: push(constant()); break;
                case Bytecode::push_current: push(lookat_); break;

                case Bytecode::push_prop: {
                    Object* obj = pop<Object*>();
                    push(obj->field(constant<string>()));
                } break;

                case Bytecode::store_current: lookat_ = pop<Object*>(); break;
                case Bytecode::store_prop: {
                    Object* obj = pop<Object*>();
                    obj->field(constant<string>()) = pop();
                } break;

                case Bytecode::pop: pop(); break;
                case Bytecode::swap: swap(); break;
                case Bytecode::dup: push(peek()); break;

                case Bytecode::comp: push(pop() == pop()); break;

                case Bytecode::jump_if: {
                    auto jump = read();
                    if(peek<bool>()) ip_ += jump;
                } break;

                case Bytecode::rjump_if: {
                    auto jump = read();
                    if(peek<bool>()) ip_ -= jump;
                } break;

                case Bytecode::jump: ip_ += read(); break;

                case Bytecode::rjump: ip_ -= read(); break;

                case Bytecode::go_link: {
                    auto dir = constant<string>();
                    Object* obj = pop<Object*>();
                    location_ = obj->follow(dir);
                } break;

                case Bytecode::look:
                    lookat_ = pop<Object*>();
                    break;

                case Bytecode::make: {
                    auto kind = pop<string>();
                    const Object* proto = prototype(kind);
                    push(clone(proto));
                } break;

                case Bytecode::call: {
                    auto sig = constant<string>();
                    Object* obj = peek<Object*>();
                    assert(obj->hasVerb(sig));
                    const auto& fn = ctx_.functions.at(sig);
                    openFrame(fn);
                } break;


                case Bytecode::ret: {
                    if(!callStack_.size()) return true;
                    closeFrame();
                } break;

                default:
                return false;
                break;
            }
        }
        return true;
    }

    Object* Run::clone(const Object* other) {
        allocated_ += 1;
        if(allocated_ >= nextGC_) collect();
        auto* obj = new Object(*other);
        obj->next_ = gcHead_;
        gcHead_ = obj;
        return obj;
    }

    Object* Run::allocate(const string& kind, const Object* prototype) {
        allocated_ += 1;
        if(allocated_ >= nextGC_) collect();

        auto* obj = prototype ? new Object(kind, prototype) : new Object(kind);
        obj->next_ = gcHead_;
        gcHead_ = obj;
        return obj;
    }

    Object* Run::allocate(const string& kind, const string& name) {
        return allocate(kind, prototype(name));
    }

    static int mark(const Value& v) {

        if(v.is<Value::Ref>()) {
            return v.as<Value::Ref>()->mark();
        }
        else if(v.is<Value::Array>()) {
            int marked = 0;
            for(const auto* obj: v.as<Value::Array>()) {
                marked += obj->mark();
            }
            return marked;
        }
        return 0;
    }

    void Run::collect() {
        allocated_ = 0;

        // TODO: unmark everything
        const Object* obj = gcHead_;
        while(obj) {
            obj->mark_ = false;
            obj = obj->next_;
        }

        for(const Value* v = stack_; v != stackTop_; ++v) { mark(*v); }
        for(const auto& [_, obj]: prototypes_) { allocated_ += obj->mark(); }

        auto** ptr = &gcHead_;
        while(*ptr) {
            if(!(*ptr)->mark_) {
                auto* garbage = *ptr;
                *ptr = garbage->next_;
                delete garbage;
            } else {
                (*ptr)->mark_ = false;
                ptr = &(*ptr)->next_;
            }
        }

        nextGC_ = allocated_ * 2;
    }
}
