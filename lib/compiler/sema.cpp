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


    bool operator==(const Value& left, const Value& right) {
        if(left.type() != right.type()) return false;
        switch (left.type()) {
            case Value::nil: return false;
            case Value::integer: return left.as<i32>() == right.as<i32>();
            case Value::real: return left.as<float>() == right.as<float>();
            case Value::text: return left.as<string>() == right.as<string>();
            case Value::property: return left.as<Property>().value == right.as<Property>().value;
            case Value::object: return left.as<Object*>() == right.as<Object*>();
            case Value::list: return left.as<Array>() == right.as<Array>();
        }
        return false;
    }

    bool operator!=(const Value& left, const Value& right) {
        return !(left == right);
    }

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
        if(!fields_.count(name)) {
            auto ptr = field_ptr(name);
            fields_[name] = ptr ? *ptr : Value(nil_tag);
        }
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

    void dump(std::ostream& out, const Value& v, int level = 0) {
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
                    v.as<Object*>()->dump(out, level+1);
                break;
            case Value::list:
                out << "[\n";
                for(const auto& i: v.as<Array>()) {
                    dump(out, i, level+2);
                    out << ",";
                }
                out << "\n]";
                break;
        }
    }

    void Object::dump(std::ostream& out, int level) const {
        for(int i = 0; i < level; ++i) out << "  ";
        out << name_;
        const Object* obj = prototype_;
        while(obj) {
            out << " < " << obj->name_;
            obj = obj->prototype_;
        }
        out << " {\n";
        for(const auto& [k, v]: flattened()) {
            for(int i = 0; i < level+1; ++i) out << "  ";
            out << "  " << k << ": " << v << "\n";
        }
        out << "}\n";
    }
}
