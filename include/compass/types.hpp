//===--------------------------------------------------------------------------------------------===
// primitives.hpp - Basic defines and typedefs
// This source is part of the Compass Engine
//
// Created on 2019-03-02 by Amy Parent <amy@amyparent.com>
// Copyright (c) 2019 Amy Parent
// Licensed under the MIT License
// =^•.•^=
//===--------------------------------------------------------------------------------------------===
#pragma once
#include <apfun/string.hpp>
#include <cstdint>
#include <vector>
#include <unordered_map>
#include <unordered_set>

namespace amyinorbit::compass {

    // using namespace tl;
    //
    // template <typename T>
    // using result = expected<T, std::string>;

    using i8 = std::int8_t;
    using u8 = std::uint8_t;

    using i16 = std::int16_t;
    using u16 = std::uint16_t;

    using i32 = std::int32_t;
    using u32 = std::uint32_t;

    using i64 = std::int64_t;
    using u64 = std::uint64_t;

    using uint = std::uint32_t;
    // using string = std::string;

    template <typename T>
    using vector = std::vector<T>;

    template <typename K, typename V>
    using map = std::unordered_map<K, V>;

    template <typename V>
    using set = std::unordered_set<V>;

    struct NonCopyable {
        NonCopyable() = default;
        NonCopyable(const NonCopyable&) = delete;
        NonCopyable& operator=(const NonCopyable&) = delete;
        NonCopyable(NonCopyable&&) = default;
        NonCopyable& operator=(NonCopyable&&) = default;
    };

    struct NonMovable {
        NonMovable() = default;
        NonMovable(NonMovable&&) = delete;
        NonMovable& operator=(NonMovable&&) = delete;
        NonMovable(const NonMovable&) = default;
        NonMovable& operator=(const NonMovable&) = default;
    };
}
