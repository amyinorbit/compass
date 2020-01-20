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
        if(v.is<string>())
            out << v.as<string>();
        else if(v.is<std::int32_t>())
            out << "'" << v.as<std::int32_t>() << "'";
        else if(v.is<nil_t>())
            out << "<nil>";
        else if(v.is<Value::Prop>())
            out << "prop/" << v.as<Value::Prop>().value;
        else if(v.is<Value::Ref>())
            out << "ref/" << v.as<Value::Ref>();
        else if(v.is<Value::Array>()) {
            out << "[";
            for(const auto& i: v.as<Value::Array>()) {
                dump_value(out, i);
                out << ",";
            }
            out << "]";
        }
    }

    void Object::dump_fields(std::ostream& out) const {
        if(prototype_) prototype_->dump_fields(out);
        for(const auto& [k, v]: fields_) {
            out << "  > " << k << ":";
            dump_value(out, v);
            out << "\n";
        }
    }

    bool Object::set_kind(const Object* kind) {
        if(kind && kind->is_kind(name_)) return false;
        prototype_ = kind;
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


    bool Object::is_kind(const string& name) const {
        if(!prototype_) return false;
        if(prototype_->name_ == name) return true;
        return prototype_->is_kind(name);
    }

    TypeDB::TypeDB(Driver& driver) : driver_(driver) {
        types_["number"] = std::make_unique<Type>(Type{Type::number, "number", nullptr});
        types_["text"] = std::make_unique<Type>(Type{Type::text, "text", nullptr});
        types_["object"] = std::make_unique<Type>(Type{Type::object, "object", nullptr});


        auto base = new_kind("object", nullptr);
        base->field("name") = {types_["text"].get(), ""};
        base->field("plural") = {types_["text"].get(), ""};
        base->field("description") = {types_["text"].get(), ""};

        auto direction = new_kind("direction", nullptr);
        direction->field("name") = {types_["text"].get(), ""};
        direction->field("opposite") = {types_["text"].get(), ""};


        auto room = new_kind("room", base);
        auto relation = new_kind("relation", nullptr);
        relation->field("direction") = {types_["direction"].get(), nullptr};
        relation->field("target") = {types_["object"].get(), nullptr};

        auto thing = new_kind("thing", base);

        room->field("directions") = {list_type(types_.at("room").get()), Value::Array()};
        // TODO: add field with type Link[]
        // room->fields["directions"] = {types_}
        // room
    }

    const Type* TypeDB::type_of(const Value& value) {
        switch(value.data.index()) {
            case 0: return types_.at("number").get();
            case 1: return types_.at("text").get();
            case 2: return prop_type_of(value.as<Value::Prop>().value);
            case 3: return obj_type_of(value.as<Object*>()); // TODO: replace with actual checking
            default: break;
        }
        return nullptr;
    }

    const Type* TypeDB::prop_type_of(const string &value) {
        return values_.count(value) ? values_.at(value) : nullptr;
    }

    const Type* TypeDB::obj_type_of(const Object* obj) {
        if(!obj) return nullptr;
        if(obj->is_abstract()) {
            return types_.count(obj->name()) ? types_.at(obj->name()).get() : nullptr;
        } else {
            return obj_type_of(obj->prototype());
        }
    }

    const Type* TypeDB::property(const string& name) {
        if(types_.count(name)) {
            auto type = types_.at(name).get();
            if(type->kind != Type::Kind::property) {
                driver_.diagnostic(Diagnostic::error(name + " is already something else"));
                return nullptr;
            }
            return type;
        }
        types_[name] = std::make_unique<Type>(Type{Type::property, name, nullptr});
        return types_[name].get();
    }

    void TypeDB::add_property_value(const Type* property, const string& name) {
        if(!property) return;
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
        types_[name] = std::make_unique<Type>(Type{Type::object, name, nullptr});
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

    const Type* TypeDB::list_type(const Type* contained) {
        auto name = "list of " + contained->name;
        if(!types_.count(name)) {
            types_[name] = std::make_unique<Type>(Type{Type::list, name, contained});
        }
        return types_.at(name).get();
    }

    const Type* TypeDB::property_of(const string& value) {
        if(!values_.count(value)) return nullptr;
        return values_.at(value);
    }

    Value TypeDB::property_val(const string& value) const {
        if(!values_.count(value)) {
            driver_.diagnostic(Diagnostic::error(value + " is not a property that I know"));
            return Value{nullptr, nil_tag};
        }
        return Value{values_.at(value), Value::Prop{value}};
    }
}
