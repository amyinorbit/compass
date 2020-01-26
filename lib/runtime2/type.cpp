//===--------------------------------------------------------------------------------------------===
// type.cpp - Implementation of TypeDB/type inference
//
// Created by Amy Parent <amy@amyparent.com>
// Copyright (c) 2020 Amy Parent
// Licensed under the MIT License
// =^•.•^=
//===--------------------------------------------------------------------------------------------===
#include <compass/runtime2/type.hpp>

namespace amyinorbit::compass {

    Object::Object(kind_t, const Object* prototype, string name)
        : prototype_(prototype)
        , is_abstract_(true)
        , name_(name) { }

    Object::Object(concrete_t, const Object* prototype, string name)
        : prototype_(prototype)
        , is_abstract_(false)
        , name_(name) { }

    void Object::dump(std::ostream& out) const {
        out << name_;
        if(is_abstract_) {
            out << "[kind]";
        }
        const Object* p = prototype_;
        while(p) {
            out << "<" << p->name_;
            p = p->prototype_;
        }
        std::cout << ":\n";
        dump_fields(out);
    }

    void dump_value(std::ostream& out, const Value& v) {
        switch(v.type()) {
            case Type::nil: out << "<nil>"; break;
            case Type::text: out << "'" << v.as<string>() << "'"; break;
            case Type::symbol: out << "unresolved ref/" << as<Value::Symbol>().value; break;
            case Type::number: out << v.as<std::int32_t>(); break;
            case Type::property: out << "prop/" << v.as<Value::Prop>().value; break;
            case Type::object:
                if(!v.as<Object*>())
                    out << "obj/nil";
                else
                    out << "obj/" << v.as<Object*>()->name();
                break;
            case Type::list:
                out << "[";
                for(const auto& i: v.as<Value::Array>()) {
                    dump_value(out, i);
                    out << ",";
                }
                out << "]";
                break;
        }
    }

    void Object::dump_fields(std::ostream& out) const {
        if(prototype_) prototype_->dump_fields(out);
        for(const auto& [k, v]: fields_) {
            out << "  > " << k << ": ";
            dump_value(out, v);
            out << "\n";
        }
    }

    bool Object::set_kind(const Object* kind) {
        if(kind && kind->is_a(this)) return false;
        prototype_ = kind;
        return true;
    }

    bool Object::has_field(const string& name) const {
        if(fields_.count(name)) return true;
        if(!prototype_) return false;
        return prototype_->has_field(name);
    }

    bool Object::has_field(const string& name, Type type) const {
        auto field = field_ptr(name);
        if(!field) return false;
        return field->type() == type;
    }

    bool Object::conforms_to(const Contract& contract) const {
        for(const auto& [k, t]: contract) {
            if(!has_field(k, t)) return false;
        }
        return true;
    }

    Value& Object::field(const string& name) {
        if(fields_.count(name)) return fields_.at(name);

        auto v = prototype_ ? prototype_->field_ptr(name) : nullptr;
        if(v) {
            fields_[name] = *v;
        } else {
            fields_[name] = Value();
        }
        return fields_[name];
    }

    const Value& Object::field(const string& name) const {
        if(fields_.count(name)) return fields_.at(name);

        if(!prototype_) abort();
        return prototype_->field(name);
    }

    const Value* Object::field_ptr(const string& name) const {
        if(fields_.count(name)) return &fields_.at(name);
        return prototype_ ? prototype_->field_ptr(name) : nullptr;
    }

    bool Object::is_a(const Object* kind) const {
        if(name_ == kind->name_) return true;
        if(!prototype_) return false;
        return prototype_->is_a(kind);
    }

    set<string> Object::field_names() const {
        set<string> all;
        field_names(all);
        return all;
    }

    void Object::field_names(set<string>& out) const {
        if(prototype_) prototype_->field_names(out);

        for(const auto& [k, _]: fields_) {
            if(out.count(k)) continue;
            out.insert(k);
        }
    }

    bool Value::can_assign(const Value &other) const {
        if(type() != other.type()) return false;

        switch(type()) {
        case Type::nil:
        case Type::text:
        case Type::symbol:
        case Type::number:
        case Type::list:
        case Type::property:
            return true;
        case Type::object:
            return other.as<Ref>()->is_a(as<Ref>());
        }
        return false;
    }
}
