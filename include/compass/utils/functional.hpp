//===--------------------------------------------------------------------------------------------===
// functional.hpp - functional-style includes
// This source is part of the Compass Engine
//
// Created on 2019-02-07 by Amy Parent <amy@amyparent.com>
// Copyright (c) 2019 Amy Parent
// Licensed under the MIT License
// =^•.•^=
//===--------------------------------------------------------------------------------------------===
#pragma once
#include <algorithm>
#include <functional>
#include <iostream>
#include <string>
#include <compass/utils/expected.hpp>
#include <compass/utils/optional.hpp>

using namespace tl;

template <typename T>
using result = expected<T, std::string>;

template <typename T>
T& maybe_guard(optional<T>& m, const std::string& message) {
    if(m) return *m;
    std::cerr << "cannot unwrap optional: " << message << "\n";
    abort();
}

template <typename T>
optional<typename T::value_type> find(const T& c, std::function<bool(typename T::value_type)> pred) {
    const auto it = std::find_if(c.begin(), c.end(), pred);
    if(it == c.end()) return {};
    return it->second;
}
