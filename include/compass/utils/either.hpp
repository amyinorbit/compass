//===--------------------------------------------------------------------------------------------===
// Either.hpp - functional-style Either type
// This source is part of the Compass Engine
//
// Created on 2019-02-07 by Amy Parent <amy@amyparent.com>
// Copyright (c) 2019 Amy Parent
// Licensed under the MIT License
// =^•.•^=
//===--------------------------------------------------------------------------------------------===
#pragma once
#include <type_traits>
#include <string>
#include <functional>
#include <compass/utils/invoke_compat.hpp>

template <typename L, typename R>
class Either {
public:
    using left_type = L;
    using right_type = R;
    
    Either(const L& value) : isLeft_(true), left_(value) {}
    Either(L&& value) : isLeft_(true), left_(std::move(value)) {}
    Either(const R& value) : isLeft_(false), right_(value) {}
    Either(R&& value) : isLeft_(false), right_(std::move(value)) {}
    
    Either(const Either& rhs) : isLeft_(rhs.isLeft_) {
        if(isLeft_)
            left_ = rhs.left_;
        else
            right_ = rhs.right_;
    }
    
    Either& operator=(const Either& rhs) {
        if(&rhs != this) {
            if(isLeft_)
                left_.~T();
            else
                right_.~E();
            
            isLeft_ = rhs.isLeft_;
            if(isLeft_)
                left_ = rhs.left_;
            else
                right_ = rhs.right_;
        }
        return *this;
    }
    
    ~Either() {
        if(isLeft_)
            left_.~L();
        else
            right_.~R();
    }
    
    const L& left() const { return left_; }
    const R& right() const { return right_; }
    
    template <typename F, std::enable_if_t<std::is_void<invoke_result_t<F, L>>{}, int> = 0>
    Either mapLeft(F&& f) const {
        if(isLeft_) {
            invoke(f, left_);
            return left_;
        }
        return right_;
    }
    
    template <typename F, std::enable_if_t<std::is_void<invoke_result_t<F, R>>{}, int> = 0>
    constexpr auto mapRight(F&& f) const {
        if(!isLeft_) {
            invoke(f, right_);
            return right_;
        }
        return left_;
    }
    
    template <typename F, std::enable_if_t<!std::is_void<invoke_result_t<F, L>>{}, int> = 0>
    constexpr auto mapLeft(F&& f) const {
        if(isLeft_) return Either<invoke_result_t<F, L>, R>(invoke(f, left_));
        return Either<invoke_result_t<F, L>, R>(right_);
    }
    
    template <typename F, std::enable_if_t<!std::is_void<invoke_result_t<F, R>>{}, int> = 0>
    constexpr auto mapRight(F&& f) const {
        if(!isLeft_) return Either<L, invoke_result_t<F, L>>(invoke(f, right_));
        return Either<L, invoke_result_t<F, R>>(left_);
    }
    
private:
    bool isLeft_;
    union {
        L left_;
        R right_;
    };
};


