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

    Object* Sema::object(const string& name) {
        auto id = name.lowercased();
        if(!objects_.count(id)) return nullptr;
        return objects_.at(id).get();
    }

    Object* Sema::create_object(const Object* proto, const string& name) {
        auto id = name.lowercased();
        objects_[id] = std::make_unique<Object>(proto, id);
        auto obj = objects_.at(id).get();
        return obj;
    }

    void Sema::write(std::ostream &out) const {
        CodeGen cg;
        for(const auto& [k, obj]: objects_) {
            cg.add_object(obj.get());
        }

        cg.write(out);
    }

    static inline void tabs(int depth) {
        while(depth--) {
            std::cout << " ";
        }
    }

#ifdef INDEX_COLORS
#define START_FIELD ""
#define END_FIELD ""
#define START_BAR "\033[1;36;7m"
#define END_BAR "\033[0m"
#else
#define START_FIELD ""
#define END_FIELD ""
#define START_BAR "["
#define END_BAR "]"
#endif

    void print_index_(const Value& val, int depth = 0);
    void print_index_(const Object* obj, int depth = 0) {
        // std::cout << "\n";
        // tabs(depth);

        std::cout << START_BAR << " " << obj->name();

        const Object* proto = obj->prototype();
        while(proto) {
            std::cout << " < " << proto->name();
            proto = proto->prototype();
        }

        std::cout << " " << END_BAR << "\n";

        std::vector<Value> defer;

        for(const auto& [k, v]: obj->flattened()) {
            if(v.is<Array>()) {
                if(v.as<Array>().size()) {
                    defer.push_back(v);
                }
                continue;
            }
            tabs(depth);
            std::cout << "| " << START_FIELD << k << END_FIELD << ": ";
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
        std::cout << START_BAR << " compass 2.0: story index " << END_BAR << "\n\n";
        // if(root_room_)
        //     print_index_(root_room_);

    }
}
