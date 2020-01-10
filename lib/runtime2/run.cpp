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
#include <iostream>
#include <compass/runtime2/run.hpp>
#include <compass/runtime2/object.hpp>
#include <compass/runtime2/bytecode.hpp>

namespace amyinorbit::compass {

    Machine::Machine(const Context& ctx) : ctx_(ctx), stack_(512), callStack_(32), heap_(1024) {
        gc_.onGC([this]{
            for(const auto& v: stack_) { gc_.mark(v); }
            for(const auto& v: heap_) { gc_.mark(v); }
            for(const auto& [_, obj]: prototypes_) { gc_.mark(obj); }
        });

        for(const auto& p: ctx.prototypes) {
            prototypes_[p.kind] = gc_.clone(&p);
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

        #define CODE(inst, block) case inst: block; break;

        while((inst = readCode()) != Bytecode::halt) {
            switch (inst) {
                CODE(Bytecode::halt, return true);
                CODE(Bytecode::load_true, stack_.push_back(true));
                CODE(Bytecode::load_false, stack_.push_back(false));
                CODE(Bytecode::load_const, stack_.push_back(constant()));
                CODE(Bytecode::load_var, stack_.push_back(heap_[read()]));

                CODE(Bytecode::load_prop, {
                    auto obj = stack_.pop_back<Value::Ref>();
                    stack_.push_back(obj->field(constant<string>()));
                });

                CODE(Bytecode::store_prop, {
                    auto obj = stack_.pop_back<Value::Ref>();
                    obj->field(constant<string>()) = stack_.pop_back();
                });

                CODE(Bytecode::store_var, {
                    heap_[read()] = stack_.pop_back();
                });

                CODE(Bytecode::pop, stack_.pop_back());
                CODE(Bytecode::swap, swap());
                CODE(Bytecode::dup, stack_.push_back(stack_.back()));

                CODE(Bytecode::io_style, {

                });

                CODE(Bytecode::io_read, {
                    std::string line;
                    std::getline(std::cin, line);
                    stack_.push_back(string(line.data(), line.size()));
                });

                CODE(Bytecode::io_write, {
                    std::cout << stack_.pop_back<string>() << "\n";
                });


                CODE(Bytecode::comp, stack_.push_back(stack_.pop_back() == stack_.pop_back()));
                CODE(Bytecode::jump_if, {
                    auto jump = read();
                    if(stack_.back<bool>()) ip_ += jump;
                });

                CODE(Bytecode::rjump_if, {
                    auto jump = read();
                    if(stack_.back<bool>()) ip_ -= jump;
                });

                CODE(Bytecode::jump, ip_ += read());
                CODE(Bytecode::rjump, ip_ -= read());

                CODE(Bytecode::link, {
                    auto dir = constant<string>();
                    auto obj = stack_.pop_back<Object*>();
                    stack_.push_back(follow(obj, dir));
                });

                CODE(Bytecode::clone, {
                    auto kind = stack_.pop_back<string>();
                    const Object* proto = prototype(kind);
                    stack_.push_back(gc_.clone(proto));
                });

                CODE(Bytecode::call, {
                    auto sig = constant<string>();
                    Object* obj = stack_.back<Object*>();
                    assert(obj->hasVerb(sig));
                    const auto& fn = ctx_.functions.at(sig);
                    openFrame(fn);
                });

                CODE(Bytecode::ret, {
                    if(!callStack_.size()) return true;
                    closeFrame();
                });

                default: return false;
            }
        }
        return true;
    }
}
