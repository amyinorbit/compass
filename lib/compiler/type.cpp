//===--------------------------------------------------------------------------------------------===
// type.cpp - Implementation of TypeDB/type inference
//
// Created by Amy Parent <amy@amyparent.com>
// Copyright (c) 2020 Amy Parent
// Licensed under the MIT License
// =^•.•^=
//===--------------------------------------------------------------------------------------------===
#include <compass/compiler/type.hpp>

namespace amyinorbit::compass::type {

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

    const Value* Object::field_ptr(const string& name) const {
        if(fields_.count(name)) return &fields_.at(name);
        return prototype_ ? prototype_->field_ptr(name) : nullptr;
    }

    bool Object::is_a(const Object* kind) const {
        if(!prototype_) return false;
        if(prototype_->name_ == kind->name_) return true;
        return prototype_->is_a(kind);
    }

    bool Value::can_assign(const Value &other) const {
        if(type() != other.type()) return false;

        switch(type()) {
        case Type::nil:
        case Type::text:
        case Type::number:
        case Type::list:
        case Type::property:
            return true;
        case Type::object:
            return other.as<Ref>()->is_a(as<Ref>());
        }
    }

    TypeDB::TypeDB(Driver& driver) : driver_(driver) {

        auto base = new_kind("object", nullptr);
        base->field("name") = "";
        base->field("plural") = "";
        base->field("description") = "";

        auto direction = new_kind("direction", nullptr);
        direction->field("name") = "";
        direction->field("opposite") = "";


        auto room = new_kind("room", base);
        auto relation = new_kind("relation", nullptr);
        relation->field("direction") = nullptr;
        relation->field("target") = nullptr;

        auto thing = new_kind("thing", base);

        room->field("directions") = Value::Array();
        room->field("children") = Value::Array();
    }

    void TypeDB::property(const string& name) {
        if(properties_.count(name)) return;
        properties_.insert(name);
    }

    void TypeDB::add_property_value(const string& property, const string& name) {
        if(!properties_.count(property)) {
            driver_.diagnostic(Diagnostic::error(name + " is not a property"));
        }
        if(values_.count(name) && values_.at(name) != property) {
            driver_.diagnostic(Diagnostic::error(name + " is already something else"));
            return;
        }
        values_[name] = property;
    }

    Object* TypeDB::new_kind(const string& name, const Object* prototype) {
        if(world_.count(name)) {
            driver_.diagnostic(Diagnostic::error("another object called " + name + " exists"));
            return nullptr;
        }
        world_[name] = std::make_unique<Object>(kind_tag, prototype, name);
        kinds_[name] = world_.at(name).get();
        return world_.at(name).get();
    }

    Object* TypeDB::new_object(const string& name, const Object* prototype) {
        if(world_.count(name)) {
            driver_.diagnostic(Diagnostic::error("another object called " + name + " exists"));
            return nullptr;
        }
        world_[name] = std::make_unique<Object>(concrete_tag, prototype, name);
        return world_.at(name).get();
    }

    Object* TypeDB::object(const string& name) {
        if(!world_.count(name)) {
            driver_.diagnostic(Diagnostic::error("I don't know anything called " + name));
            return nullptr;
        }
        return world_.at(name).get();
    }

    Object* TypeDB::fetch_or_create(const string& name) {
        if(!world_.count(name)) {
            return new_object(name, world_.at("object").get());
        } else {
            return world_.at(name).get();
        }
    }

    maybe<string> TypeDB::property_of(const string& value) {
        if(!values_.count(value)) return nothing();
        return values_.at(value);
    }
}
