//===--------------------------------------------------------------------------------------------===
// Maybe.hpp - Maybe/Optional type
// This source is part of the Compass Engine
//
// Created on 2019-02-07 by Amy Parent <amy@amyparent.com>
// Copyright (c) 2019 Amy Parent
// Licensed under the MIT License
// =^•.•^=
//===--------------------------------------------------------------------------------------------===
#pragma once
#include <type_traits>
#include <functional>

template <typename T>
class Maybe final {
public:
    using wrapped_type = T;
    
    Maybe() : engaged_(false) {}
    Maybe(const T& some) : engaged_(true) { new(&storage_) T(some); }
    Maybe(T&& some) : engaged_(true) { new(&storage_) T(std::move(some)); }
    
    // Copy Constructors
    // TODO: add move constructors
    
    Maybe(const Maybe& rhs) : engaged_(rhs.engaged_) {
        if(engaged_) new(&storage_) T(rhs.get());
    }
    
    Maybe& operator=(const Maybe& rhs) {
        if(&rhs != this) {
            engaged_ = rhs.engaged_;
            new(&storage_) T(rhs.get());
        }
        return *this;
    }
    
    ~Maybe() {
        if(!engaged_) reinterpret_cast<T*>(&storage_)->~T();
    }
    
    operator bool() { return engaged_; }
    operator T() { return *reinterpret_cast<const T*>(&storage_); }
    const T& get() const { return *reinterpret_cast<const T*>(&storage_); }
    
    template <typename U>
    Maybe flatMap(std::function<Maybe<U>(T)> transform) {
        if(!engaged_) return {};
        return transform(get());
    }
    
    template <typename U>
    Maybe<U> map(std::function<U(T)> transform) {
        if(!engaged_) return {};
        return transform(get());
    }
    
private:
    bool engaged_;
    
    T& some() { return *reinterpret_cast<const T*>(&storage_); }
    std::aligned_storage_t<sizeof(T), alignof(T)> storage_;
};