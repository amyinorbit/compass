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

    TypeDB::TypeDB(Driver& driver) : driver_(driver) {
        types_["number"] = std::make_unique<Type>(Type{Type::number, "number", nullptr});
        types_["text"] = std::make_unique<Type>(Type{Type::text, "text", nullptr});
        types_["object"] = std::make_unique<Type>(Type{Type::object, "object", nullptr});

        auto base = std::make_unique<Object>(nullptr, true, "object");
        base->fields["name"] = {types_["text"].get(), ""};
        base->fields["plural"] = {types_["text"].get(), ""};
        base->fields["description"] = {types_["text"].get(), ""};
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
        if(obj->is_abstract) {
            return types_.count(obj->name) ? types_.at(obj->name).get() : nullptr;
        } else {
            return obj_type_of(obj->prototype);
        }
    }

    const Type* TypeDB::new_property(const string& name) {
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
        world_[name] = std::make_unique<Object>(Object{prototype, true, name});
        types_[name] = std::make_unique<Type>(Type{Type::object, name, nullptr});
        return world_.at(name).get();
    }

    Object* TypeDB::new_object(const string& name, const Object* prototype) {
        if(world_.count(name)) {
            driver_.diagnostic(Diagnostic::error("another object called " + name + " exists"));
            return nullptr;
        }
        world_[name] = std::make_unique<Object>(Object{prototype, false, name});
        return world_.at(name).get();
    }

    Object* TypeDB::object(const string& name) {
        return world_.count(name) ? world_.at(name).get() : nullptr;
    }

    const Value* TypeDB::obj_field(const Object* obj, const string& name) const {
        if(!obj) return nullptr;
        if(obj->fields.count(name)) return &obj->fields.at(name);

        const Value* v = proto_field(obj->prototype, name);
        return v;
    }

    Value* TypeDB::obj_field(Object* obj, const string& name) {
        if(!obj) return nullptr;
        if(obj->fields.count(name)) return &obj->fields.at(name);

        const Value* v = proto_field(obj->prototype, name);
        if(!v) return nullptr;
        obj->fields[name] = *v;
        return &obj->fields.at(name);
    }

    const Value* TypeDB::proto_field(const Object* obj, const string& name) const {
        if(!obj) return nullptr;
        if(obj->fields.count(name)) return &obj->fields.at(name);
        return proto_field(obj->prototype, name);
    }

    const Type* TypeDB::property_of(const string& value) {
        if(!values_.count(value)) return nullptr;
        return values_.at(value);
    }
}
