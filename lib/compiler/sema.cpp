//===--------------------------------------------------------------------------------------------===
// sema.cpp - Semantic type system for the compiler
//
// Created by Amy Parent <amy@amyparent.com>
// Copyright (c) 2019 Amy Parent
// Licensed under the MIT License
// =^•.•^=
//===--------------------------------------------------------------------------------------------===
#include <compass/compiler/sema.hpp>

namespace amyinorbit::compass::sema {

    Object::Object(const Object* prototype, const string& name)
        : prototype_(prototype), name_(name) {

    }

    bool Object::set_prototype(const Object* prototype) {
        if(prototype) {
            if(prototype->is_a(this)) return false;
            if(is_a(prototype)) return true;
        }
        prototype_ = prototype;
        return true;
    }

    bool Object::is_a(const Object* kind) const {
        if(kind == this) return true;
        if(kind->name_ == name_) return true;
        if(!prototype_) return false;
        return prototype_->is_a(kind);
    }

    bool Object::has_field(const string& name) const {
        if(fields_.count(name)) return true;
        if(!prototype_) return false;
        return prototype_->has_field(name);
    }

    Value& Object::field(const string& name) {
        return fields_.at(name);
    }

    const Value& Object::field(const string& name) const {
        auto ptr = field_ptr(name);
        if(!ptr) abort();
        return *ptr;
    }

    const Value* Object::field_ptr(const string& name) const {
        if(fields_.count(name)) return &fields_.at(name);
        if(!prototype_) return nullptr;
        return prototype_->field_ptr(name);
    }

    Object::FlatRepr Object::flattened() const {
        FlatRepr flat;
        flatten(flat);
        return flat;
    }

    void Object::flatten(FlatRepr& flat) const {
        if(prototype_) prototype_->flatten(flat);
        for(const auto& [k, v]: fields_) {
            flat[k] = v;
        }
    }

    std::ostream& operator<<(std::ostream& out, const Value& v) {
        switch(v.type()) {
            case Value::nil: out << "<nil>"; break;
            case Value::text: out << "'" << v.as<string>() << "'"; break;
            case Value::integer: out << v.as<i32>(); break;
            case Value::real: out << v.as<float>(); break;
            case Value::property: out << "prop/" << v.as<Property>().value; break;
            case Value::object:
                if(!v.as<Object*>())
                    out << "obj/nil";
                else
                    v.as<Object*>()->dump(out);
                break;
            case Value::list:
                out << "[";
                for(const auto& i: v.as<Array>()) {
                    out << i << ",";
                }
                out << "]";
                break;
        }
        return out;
    }

    void Object::dump(std::ostream& out) const {
        out << name_;
        const Object* obj = prototype_;
        while(obj) {
            out << " < " << obj->name_;
            obj = obj->prototype_;
        }
        out << " {\n";
        for(const auto& [k, v]: flattened()) {
            out << "  - " << k << ": " << v << "\n";
        }
        out << "}\n";
    }
}
