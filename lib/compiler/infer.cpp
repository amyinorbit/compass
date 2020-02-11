//===--------------------------------------------------------------------------------------------===
// infer.cpp - Inference engine implementation
//
// Created by Amy Parent <amy@amyparent.com>
// Copyright (c) 2019 Amy Parent
// Licensed under the MIT License
// =^•.•^=
//===--------------------------------------------------------------------------------------------===
#include <compass/compiler/infer.hpp>
#include <compass/compiler/codegen.hpp>
#include <boost/bimap/support/lambda.hpp>
#include <apfun/view.hpp>

namespace amyinorbit::compass {
    using namespace sema;

    template <typename F>
    struct map_helper_t {
        map_helper_t(F&& f) : f(std::forward<F>(f)) {}
        F&& f;
    };

    template <typename F>
    struct fail_helper_t {
        fail_helper_t(F&& f) : f(std::forward<F>(f)) {}
        F&& f;
    };

    template <typename F>
    constexpr map_helper_t<F> fmap(F&& f) { return map_helper_t<F>(std::forward<F>(f)); }

    template <typename F>
    constexpr fail_helper_t<F> fail(F&& f) { return fail_helper_t<F>(std::forward<F>(f)); }

    template <typename T, typename F, std::invoke_result_t<F, T*>* = nullptr>
    T* operator | (T* value, const map_helper_t<F>& m) {
        if(value != nullptr) m.f(value);
        return value;
    }

    template <typename T, typename F>
    T* operator | (T* value, const fail_helper_t<F>& m) {
        if(value == nullptr) m.f();
        return value;
    }

    InferEngine::InferEngine(Driver& driver, sema::Sema& sema) : driver_(driver), sema_(sema) {
        set_plural("thing", "things");
        set_plural("room", "rooms");
        set_plural("object", "objects");
    }

    void InferEngine::select(const string& what) {
        ref_ = Ref{what, nothing()};
    }

    void InferEngine::select(const string& what, const string& field) {
        ref_ = Ref{what, field};
    }

    void InferEngine::new_kind(const string& prototype_name) {
        if(error(!ref_, "I am not sure what you are referring to")) return;
        if(error(ref_->field, "A property of something cannot be a new kind")) return;

        world_.kind(prototype_name)
            | fmap([&](Object* prototype) {
               //sema_.create_kind(prototype, ref_->obj);
            })
            | fail([&] {
                error("'" + prototype_name + "' is not a kind that I know of");
            });
    }

    void InferEngine::new_property(const string& prototype) {
        if(error(!ref_, "I am not sure what you are referring to")) return;
        if(error(ref_->field, "A property of something cannot be a new kind of property")) return;

        if(!world_.exists(ref_->obj)) sema_.create_property(ref_->obj);

        auto obj = sema_.object(prototype);
        if(!obj) return;
        obj->field(ref_->obj) = Property{"<not set>"};
    }

    void InferEngine::new_property() {
        if(error(!ref_, "I am not sure what you are referring to")) return;
        if(error(ref_->field, "A property of something cannot be a new kind of property")) return;
        // if(error(sema_.pro, "A property")) return;
        sema_.create_property(ref_->obj);
        set_plural(ref_->obj, ref_->obj + "s");
    }

    void InferEngine::new_property_value(const string& property_name) {
        if(error(!ref_, "I am not sure what you are referring to")) return;
        if(error(ref_->field, "A property of something cannot be a new value of a property")) return;
        if(!world_.exists(property_name)) {
            sema_.create_property(property_name);
        }

        if(!ensure_not_exists(ref_->obj)) return;
        sema_.create_value(property_name, ref_->obj);
    }

    void InferEngine::set_kind(const string& kind_name) {
        if(error(!ref_, "I am not sure what you are referring to")) return;
        if(error(ref_->field, "A property of something cannot be a new kind of property")) return;
        auto new_kind = world_.kind(kind_name);
        if(!new_kind) return;

        if(world_.exists(ref_->obj)) {
            auto obj = sema_.object(ref_->obj);
            if(error(!obj->set_prototype(new_kind), "I can't set the kind to " + kind_name)) return;
        } else {
            sema_.create_object(new_kind, ref_->obj);
            set_plural(ref_->obj, ref_->obj + "s");
        }
    }

    bool InferEngine::is_a(const string& what) {
        if(error(!ref_, "I am not sure what you are referring to")) return false;
        if(error(ref_->field, "A property of something cannot be a new kind of property")) return false;

        auto type = type_of(what);

        switch(type) {
        case Value::object:
            set_kind(what);
            break;
        case Value::property:
            new_property_value(what);
            break;
        default:
            error("There is no such thing as " + what);
            break;
        }
        return type == Value::object;
    }

    void InferEngine::declare_property(const string& property_name, const string& value) {
        if(!exists(property_name)) {
            sema_.create_property(property_name);
            set_plural(property_name, property_name + "s");
        }

        if(!ensure_not_exists(value)) return;
        sema_.create_value(property_name, value);
    }

    void InferEngine::contained(const string& how, const string& in_what) {
        if(error(!ref_, "I am not sure what you are referring to")) return;
        if(error(ref_->field, "A property of something cannot be a new kind of property")) return;

        Object *container, *containee;
        if(!(containee = object(ref_->obj))) return;
        if(!(container = object(in_what))) return;

        if(error(!container->has_field("children"), in_what + " is not a container")) return;

        auto& children = container->field("children");
        if(error(!children.is<Array>(), "invalid container")) return;

        children.as<Array>().push_back(Value(containee));
    }

    void InferEngine::link_to(const string& direction, const string& place) {

    }

    void InferEngine::set_property(const string& value) {
        if(error(!ref_, "I am not sure what you are referring to")) return;

        auto obj = world_.object(ref_->obj);
        if(!obj) return;

        auto prop = sema_.property_of(value);
        if(prop) {
            if(error(ref_->field && ref_->field != prop, value + " is not a " + *prop)) return;
            obj->field(*prop) = Property{value};
        } else {
            if(error(!ref_->field, "I cannot make an object into text")) return;
            if(*ref_->field == "plural") set_plural(ref_->obj, value);
            obj->field(*ref_->field) = value;
        }
    }

    void InferEngine::set_plural(const string& singular, const string& plural) {

        using bm_type = bimap<string, string>;
        using value_type = bm_type::value_type;

        auto low_singular = singular.lowercased();
        auto low_plural = plural.lowercased();

        auto it = plurals_.left.find(low_singular);
        if(it != plurals_.left.end()) {
            plurals_.left.replace_data(it, low_plural);
        } else {
            plurals_.insert(value_type(low_singular, low_plural));
        };
    }
}
