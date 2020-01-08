//===--------------------------------------------------------------------------------------------===
// object.hpp - Base interface of Compass language objects
// This source is part of the Compass Engine
//
// Created on 2019-03-03 by Amy Parent <amy@amyparent.com>
// Copyright (c) 2019 Amy Parent
// Licensed under the MIT License
// =^•.•^=
//===--------------------------------------------------------------------------------------------===
#pragma once
#include <compass/runtime2/primitives.hpp>
#include <compass/runtime2/value.hpp>

namespace Compass::rt2 {

    class Object;

    struct Link {
        Object* target;
        string direction;
    };


    class Object: NonMovable, NonCopyable {
    public:
        Object(const Object& other);
        Object(const string& kind) : kind_(kind) {}
        Object(const string& kind, const Object* proto)
            : kind_(kind)
            , prototype_(proto)
            , properties_(proto->properties_)
            , links_(proto->links_)
            , verbs_(proto->verbs_) {}

        template <typename T>
        void addField(const string& name) { properties_[name] = T(); }

        template <typename T>
        void addField(const string& name, const T& value) { properties_[name] = value; }

        void addVerb(const string& verb) { verbs_.insert(verb); }
        bool hasVerb(const string& verb) const { return verbs_.count(verb); }

        const vector<Link>& links() const { return links_; }
        Object* follow(const string& direction) const;
        void link(Object* target, const string& direction) { links_.push_back({target, direction}); }

        Value& property(const string& name);
        const Value& property(const string& name) const;

        Value& operator[](const string& name) { return properties_[name]; }
        const Value& operator[](const string& name) const { return properties_.at(name); }

        Value& field(const string& name) { return properties_[name]; }
        const Value& field(const string& name) const { return properties_.at(name); }

        bool isa(const string& kind) const;
        const string& kind() const { return kind_; }

        int mark() const;
        bool isMarked() const { return mark_; }

    private:
        friend class Machine;

        mutable bool    mark_   = false;
        mutable Object* next_   = nullptr;

        const string kind_;
        const Object* const prototype_ = nullptr;
        map<string, Value> properties_;
        vector<Link> links_;
        set<string> verbs_;
    };
}
