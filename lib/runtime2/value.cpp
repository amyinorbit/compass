//===--------------------------------------------------------------------------------------------===
// value.cpp - Implementation of Compass's 2.0 runtime value representation
//
// Created by Amy Parent <amy@amyparent.com>
// Copyright (c) 2019 Amy Parent
// Licensed under the MIT License
// =^•.•^=
//===--------------------------------------------------------------------------------------------===
#include <compass/runtime2/value.hpp>
#include <compass/runtime2/object.hpp>

namespace amyinorbit::compass {
    Value::operator bool() const {
        return isTrue();
    }

    bool Value::isTrue() const {
        if(is<nil_t>()) return false;
        if(is<bool>()) return as<bool>();
        if(is<double>()) return as<double>() != 0.0;
        if(is<Ref>()) return as<Ref>() != nullptr;
        return false;
    }

    const Value operator+(const Value& left, const Value& right) {
        if(left.index() != right.index()) return nil;
        if(left.is<string>()) return left.as<string>() + right.as<string>();
        if(left.is<double>()) return left.as<double>() + right.as<double>();
        return nil;
    }

    const Value operator-(const Value& left, const Value& right) {
        if(left.index() != right.index()) return nil;
        if(left.is<double>()) return left.as<double>() - right.as<double>();
        return nil;
    }

    const Value operator*(const Value& left, const Value& right) {
        if(left.index() != right.index()) return nil;
        if(left.is<double>()) return left.as<double>() * right.as<double>();
        return nil;
    }

    const Value operator/(const Value& left, const Value& right) {
        if(left.index() != right.index()) return nil;
        if(left.is<double>()) return left.as<double>() / right.as<double>();
        return nil;
    }

    void printArray(std::ostream& out, const Value::Array& array) {
        out << "[";
        for(std::size_t i = 0; i < array.size(); ++i) {
            print(array[i], out);
            if(i < array.size() - 1) out << ", ";
        }
        out << "]";
    }

    void Value::print(std::ostream& out) const {
        // std::variant<nil_t, bool, double, string, Ref, Array> storage;
        switch(storage.index()) {

            case 0: out << "<nil>"; break;
            case 1: out << (as<bool>() ? "true" : "false"); break;
            case 2: out << as<double>(); break;
            case 3: out << as<string>(); break;
            case 4: out << as<Enum>().value; break;
            case 5: ::amyinorbit::compass::print(as<Ref>(), out); break;
            case 6: printArray(out, as<Array>()); break;

            default: out << "<invalid value>" << "\n"; break;
        }
    }
}
