//===--------------------------------------------------------------------------------------------===
// sema.cpp - Semantic checker for CNL and CASM
//
// Created by Amy Parent <amy@amyparent.com>
// Copyright (c) 2019 Amy Parent
// Licensed under the MIT License
// =^•.•^=
//===--------------------------------------------------------------------------------------------===
#include <compass/compiler/sema.hpp>
#include <compass/compiler/codegen.hpp>
#include <apfun/view.hpp>

namespace amyinorbit::compass::sema {
    using namespace fp;

    Sema::Sema(Driver& driver)  : driver_(driver) {
        create_property("size");

        for(const auto& s: {"small", "large", "massive", "tiny"}) {
            create_value("size", s);
        }

        auto base = create_kind(nullptr, "object");
        base->field("name") = "";
        base->field("plural") = "";
        base->field("description") = "";

        auto direction = create_kind(nullptr, "direction");
        direction->field("name") = "";
        direction->field("opposite") = "";

        room_kind_ = create_kind(base, "room");
        auto relation = create_kind(nullptr, "relation");
        relation->field("direction") = nullptr;
        relation->field("target") = nullptr;

        auto thing = create_kind(base, "thing");

        room_kind_->field("directions") = Array();
        room_kind_->field("children") = Array();

        auto verb = create_kind(nullptr, "verb");
        verb->field("present") = "";
        verb->field("past") = "";
        verb->field("participle") = "";
        verb->field("infinitive") = "";
    }

    bool Sema::ensure_not_exists(const string& name) {
        if(exists(name)) {
            driver_.diagnostic(Diagnostic::error(name + " already refers to something"));
            return false;
        }
        return true;
    }

    Object* Sema::object(const string& name) {
        if(error(!objects_.count(name), "There are no objects called " + name)) return nullptr;
        return objects_.at(name).get();
    }

    Object* Sema::kind(const string& name) {
        if(error(!kinds_.count(name), "There are no objects called " + name)) return nullptr;
        return kinds_.at(name).get();
    }

    maybe<string> Sema::property_of(const string& value) const {
        if(!values_.count(value)) {
            return nothing();
        }
        return values_.at(value);
    }

    Object* Sema::create_object(const Object* proto, const string& name) {
        if(!ensure_not_exists(name)) return nullptr;

        objects_[name] = std::make_unique<Object>(proto, name);
        auto obj = objects_.at(name).get();
        if(obj->has_field("name")) obj->field("name") = name;
        if(obj->has_field("plural")) obj->field("plural") = name + "s";
        world_[name] = obj;

        if(!root_room_ && proto == room_kind_) root_room_ = obj;

        return obj;
    }

    Object* Sema::create_kind(const Object* proto, const string& name) {
        if(!ensure_not_exists(name)) return nullptr;

        kinds_[name] = std::make_unique<Object>(proto, name);
        auto obj = kinds_.at(name).get();
        if(obj->has_field("name")) obj->field("name") = name;
        if(obj->has_field("plural")) obj->field("plural") = name + "s";
        world_[name] = obj;
        return obj;
    }

    void Sema::create_property(const string& name) {
        if(!ensure_not_exists(name)) return;

        properties_.insert(name);
        world_[name] = Property{""};
    }

    void Sema::create_value(const string& property, const string& value) {
        values_[value] = property;
        world_[value] = Property{value};
    }

    Object* Sema::create_verb(const string &present) {
        return create_verb(present, present + "ed", present + "ed", present + "ing");
    }

    Object* Sema::create_verb(
        const string &present,
        const string &infinitive,
        const string &past,
        const string &participle
    ) {
        Object* cl = object("verb");
        if(!cl) return nullptr;
        Object* verb = create_object(cl, present);
        if(!verb) return nullptr;
        verb->field("present") = present;
        verb->field("past") = past;
        verb->field("participle") = participle;
        verb->field("infinitive") = infinitive;

        verbs_[present] = verb;
        return verb;
    }

    void Sema::write(std::ostream &out) const {
        CodeGen cg;
        for(const auto& [k, obj]: objects_) {
            cg.add_object(obj.get());
        }
        for(const auto& [k, obj]: kinds_) {
            cg.add_object(obj.get());
        }

        cg.write(out);
    }

    static inline void tabs(int depth) {
        while(depth--) {
            std::cout << " ";
        }
    }

    void print_index_(const Value& val, int depth = 0);
    void print_index_(const Object* obj, int depth = 0) {
        // std::cout << "\n";
        // tabs(depth);

        std::cout << "\033[1;36;7m" << " " << obj->name() << "\033[0m" << "\033[36;7m";

        const Object* proto = obj->prototype();
        while(proto) {
            std::cout << " < " << proto->name();
            proto = proto->prototype();
        }

        std::cout << " " << "\033[0m" << "\n";

        std::vector<Value> defer;

        for(const auto& [k, v]: obj->flattened()) {
            if(v.is<Array>()) {
                if(v.as<Array>().size()) {
                    defer.push_back(v);
                }
                continue;
            }
            tabs(depth);
            std::cout << "| " << "\033[35m" << k << "\033[0m" << ": ";
            print_index_(v, depth+1);
            std::cout << "\n";
        }

        for(const auto& v: defer) {
            print_index_(v, depth+2);
        }
    }

    void print_index_(const Value& val, int depth) {
        switch(val.type()) {
        case Value::nil:
            tabs(depth);
            std::cout << "<nil>";
            break;

        case Value::integer:
            std::cout << val.as<i32>();
            break;

        case Value::real:
            std::cout << val.as<float>();
            break;

        case Value::text:
            std::cout << "'" << val.as<string>() << "'";
            break;

        case Value::property:
            std::cout << "prop:" << val.as<Property>().value;
            break;

        case Value::list:
            if(val.as<Array>().size()) {
                std::cout << "\n";
                for(const auto& v: val.as<Array>()) {
                    tabs(depth+1);
                    std::cout << "- ";
                    print_index_(v, depth+2);
                    std::cout << "\n";
                }
            } else {
                std::cout << "[]";
            }
            break;

        case Value::object:
            print_index_(val.as<Ref>(), depth+1);
            break;

        }
    }

    void Sema::print_index() const {
        std::cout << "\033[1;30;7m" << " compass 2.0: story index " << "\033[0m" << "\n\n";
        if(root_room_)
            print_index_(root_room_);

    }
}
