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

namespace Compass::rt2 {

    u16 Run::read() const {
        return *(ip_++);
    }

    Run::Code Run::readCode() const {
        return static_cast<Code>(read());
    }

    Run::Run(const Context& ctx) : ctx_(ctx) {
        for(const auto& p: ctx.prototypes) {
            prototypes_[p.kind()] = clone(&p);
        }
        // auto obj = allocate("Object");
        // obj->addField<string>("description");
        // obj->addField<string>("name");
        // obj->addField<string>("article");
        // obj->addField<Object*>("parent", nullptr);
        // obj->addField<vector<Object*>>("children");
        // prototypes_["Object"] = obj;
        //
        // auto room = allocate("Room", obj);
        // room->addField<bool>("visited");
        // prototypes_["Room"] = room;
        //
        // prototypes_["Thing"] = allocate("Thing", obj);
        // prototypes_["Container"] = allocate("Container", prototype("Thing"));
    }

    bool Run::run(const string& signature) {
        if(!ctx_.functions.count(signature)) return false;
        const auto& fn = ctx_.functions.at(signature);
        if(fn.kind() == Function::Kind::Foreign) return false;

        ip_ = fn.ip();
        Code inst = Code::halt;

        while((inst = readCode()) != Code::halt) {
            switch (inst) {
                case Code::halt: return true;

                case Code::push_true: push<bool>(true); break;
                case Code::push_false: push<bool>(false); break;
                case Code::push_const: push(constant()); break;
                case Code::push_current: push(lookat_); break;

                case Code::push_prop: {
                    Object* obj = pop<Object*>();
                    push(obj->field(constant<string>()));
                } break;

                case Code::store_current: lookat_ = pop<Object*>(); break;
                case Code::store_prop: {
                    Object* obj = pop<Object*>();
                    obj->field(constant<string>()) = pop();
                } break;

                case Code::pop: pop(); break;
                case Code::swap: swap(); break;
                case Code::dup: push(peek()); break;

                case Code::comp: push(pop() == pop()); break;

                case Code::jump_if: {
                    auto jump = read();
                    if(peek<bool>()) ip_ += jump;
                } break;

                case Code::rjump_if: {
                    auto jump = read();
                    if(peek<bool>()) ip_ -= jump;
                } break;

                case Code::jump: ip_ += read(); break;
                case Code::rjump: ip_ -= read(); break;

                case Code::go_link: break;
                case Code::look: break;
                case Code::make: {
                    auto kind = pop<string>();
                    const Object* proto = prototype(kind);
                    push(clone(proto));
                } break;

                case Code::verb: {
                    auto sig = constant<string>();
                    Object* obj = peek<Object*>();
                    assert(obj->hasVerb(sig));
                    const auto& fn = ctx_.functions.at(sig);
                    if(fn.kind() == Function::Kind::Foreign) {
                        // TODO: run the function
                    } else {
                        // TODO: push a new frame
                    }
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

        for(const auto& value: stack_) { mark(value); }
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
