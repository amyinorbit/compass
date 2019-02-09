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
#include <iostream>
#include <type_traits>
#include <functional>
#include <compass/utils/invoke_compat.hpp>

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
    
    operator bool() const { return engaged_; }
    operator T() const { return *reinterpret_cast<const T*>(&storage_); }
    const T& get() const { return *reinterpret_cast<const T*>(&storage_); }
    T& get() { return *reinterpret_cast<T*>(&storage_); }
    
    const T& operator*() const { return *reinterpret_cast<const T*>(&storage_); }
    T& operator*() { return *reinterpret_cast<T*>(&storage_); }
    
    template <typename F>
    constexpr auto flatMap(F&& f) const {
        if(!engaged_) return invoke_result_t<F,T>{};
        return invoke(f, get());
    }
    
    template <typename F, std::enable_if_t<std::is_void<invoke_result_t<F, T>>{}, int> = 0>
    Maybe map(F&& f) const {
        if(engaged_) {
            invoke(f, get());
            return get();
        }
        return {};
    }
    
    template <typename F, std::enable_if_t<!std::is_void<invoke_result_t<F, T>>{}, int> = 0>
    Maybe map(F&& f) const {
        if(engaged_) return invoke(f, get());
        return {};
    }
    
private:
    bool engaged_;
    
    T& some() { return *reinterpret_cast<const T*>(&storage_); }
    std::aligned_storage_t<sizeof(T), alignof(T)> storage_;
};

template <typename T>
const T& maybe_guard(const Maybe<T>& m, const std::string& message) {
    if(m) return m.get();
    std::cerr << "cannot unwrap optional: " << message << "\n";
    abort();
}

template <typename T>
T& maybe_guard(Maybe<T>& m, const std::string& message) {
    if(m) return m.get();
    std::cerr << "cannot unwrap optional: " << message << "\n";
    abort();
}
