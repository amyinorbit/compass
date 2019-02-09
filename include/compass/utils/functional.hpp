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
