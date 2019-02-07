//===--------------------------------------------------------------------------------------------===
// Result.hpp - functional-style Result type
// This source is part of the Compass Engine
//
// Created on 2019-02-07 by Amy Parent <amy@amyparent.com>
// Copyright (c) 2019 Amy Parent
// Licensed under the MIT License
// =^•.•^=
//===--------------------------------------------------------------------------------------------===
#pragma once
#include <string>
#include <functional>

struct Error {
    explicit Error(const std::string& description) : description(description) {}
    std::string description;
};

template <typename T, typename E = Error>
class Result {
public:
    using wrapped_type = T;
    using error_type = E;
    
    Result(const T& value) : engaged_(true), value_(value) {}
    Result(T&& value) : engaged_(true), value_(std::move(value)) {}
    Result(const E& error) : engaged_(false), error_(error) {}
    Result(E&& error): engaged_(false), error_(std::move(error)) {}
    
    Result(const Result& rhs) : engaged_(rhs.engaged_) {
        if(engaged_)
            value_ = rhs.value_;
        else
            error_ = rhs.error_;
    }
    
    Result& operator=(const Result& rhs) {
        if(&rhs != this) {
            if(engaged_)
                value_.~T();
            else
                error_.~E();
            
            engaged_ = rhs.engaged_;
            if(engaged_)
                value_ = rhs.value_;
            else
                error_ = rhs.error_;
        }
        return *this;
    }
    
    ~Result() {
        if(engaged_)
            value_.~T();
        else
            error_.~E();
    }
    
    operator bool() {
        return engaged_;
    }
    
    const T& value() const { return value_; }
    const E& error() const { return error_; }
    
    template <typename U>
    Result<U, E> map(std::function<U(T)> transform) const {
        if(engaged_) return transform(value_);
        return error_;
    }
    
    Result<T, E> map(std::function<T(T)> transform) const {
        if(engaged_) return transform(value_);
        return error_;
    }
    
    Result map(std::function<void(T)> transform) const {
        if(engaged_) {
            transform(value_);
            return value_;
        }
        return error_;
    }
    
    template <typename U>
    Result<U, E> flatMap(std::function<Result<U, E>(T)> transform) const {
        if(engaged_) return transform(value_);
        return error_;
    }
    
    Result<T, E> flatMap(std::function<Result(T)> transform) const {
        if(engaged_) return transform(value_);
        return error_;
    }
    
    Result mapError(std::function<void(E)> fn) const {
        if(engaged_) return value_;
        fn(error_);
        return error_;
    }
    
private:
    bool engaged_;
    union {
        E error_;
        T value_;
    };
};


