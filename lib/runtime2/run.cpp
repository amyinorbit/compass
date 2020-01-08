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


    Machine::Machine(const Context& ctx) : ctx_(ctx), stack_(512), callStack_(32) {
        for(const auto& p: ctx.prototypes) {
            prototypes_[p.kind()] = clone(&p);
        }
    }

    u16 Machine::read() const {
        return *(ip_++);
    }


    void Machine::openFrame(const Function& fn) {
        Frame f;
        f.ip = ip_;
        f.base = stack_.end();
        callStack_.push_back(f);
        ip_ = fn.ip();
    }

    void Machine::closeFrame() {
        assert(callStack_.size());
        const auto& f = callStack_.back();
        callStack_.pop_back();
        stack_.erase(f.base, stack_.end());
        ip_ = f.ip;
    }

    Bytecode Machine::readCode() const {
        return static_cast<Bytecode>(read());
    }

    bool Machine::run(const string& signature) {
        if(!ctx_.functions.count(signature)) return false;
        const auto& fn = ctx_.functions.at(signature);

        ip_ = fn.ip();
        Bytecode inst = Bytecode::halt;

        while((inst = readCode()) != Bytecode::halt) {
            switch (inst) {
                case Bytecode::halt: return true;

                case Bytecode::push_true: stack_.push_back<bool>(true); break;
                case Bytecode::push_false: stack_.push_back<bool>(false); break;
                case Bytecode::push_const: stack_.push_back(constant()); break;
                case Bytecode::push_current: stack_.push_back(lookat_); break;

                case Bytecode::push_prop: {
                    Object* obj = stack_.pop_back<Object*>();
                    stack_.push_back(obj->field(constant<string>()));
                } break;

                case Bytecode::store_current: lookat_ = stack_.pop_back<Object*>(); break;
                case Bytecode::store_prop: {
                    Object* obj = stack_.pop_back<Object*>();
                    obj->field(constant<string>()) = stack_.pop_back();
                } break;

                case Bytecode::pop: stack_.pop_back(); break;
                case Bytecode::swap: swap(); break;
                case Bytecode::dup: stack_.push_back(stack_.back()); break;

                case Bytecode::comp: stack_.push_back(stack_.pop_back() == stack_.pop_back()); break;

                case Bytecode::jump_if: {
                    auto jump = read();
                    if(stack_.back<bool>()) ip_ += jump;
                } break;

                case Bytecode::rjump_if: {
                    auto jump = read();
                    if(stack_.back<bool>()) ip_ -= jump;
                } break;

                case Bytecode::jump: ip_ += read(); break;

                case Bytecode::rjump: ip_ -= read(); break;

                case Bytecode::go_link: {
                    auto dir = constant<string>();
                    Object* obj = stack_.pop_back<Object*>();
                    location_ = obj->follow(dir);
                } break;

                case Bytecode::look:
                    lookat_ = stack_.pop_back<Object*>();
                    break;

                case Bytecode::make: {
                    auto kind = stack_.pop_back<string>();
                    const Object* proto = prototype(kind);
                    stack_.push_back(clone(proto));
                } break;

                case Bytecode::call: {
                    auto sig = constant<string>();
                    Object* obj = stack_.back<Object*>();
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

    Object* Machine::clone(const Object* other) {
        allocated_ += 1;
        if(allocated_ >= nextGC_) collect();
        auto* obj = new Object(*other);
        obj->next_ = gcHead_;
        gcHead_ = obj;
        return obj;
    }

    Object* Machine::allocate(const string& kind, const Object* prototype) {
        allocated_ += 1;
        if(allocated_ >= nextGC_) collect();

        auto* obj = prototype ? new Object(kind, prototype) : new Object(kind);
        obj->next_ = gcHead_;
        gcHead_ = obj;
        return obj;
    }

    Object* Machine::allocate(const string& kind, const string& name) {
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

    void Machine::collect() {
        allocated_ = 0;

        // TODO: unmark everything
        const Object* obj = gcHead_;
        while(obj) {
            obj->mark_ = false;
            obj = obj->next_;
        }

        for(const auto& v: stack_) { mark(v); }
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
