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
#include <cstdint>
#include <string>
#include <vector>
#include <unordered_map>
#include <compass/type/optional.hpp>
#include <compass/type/expected.hpp>

namespace Compass {
    
    using namespace tl;

    template <typename T>
    using result = expected<T, std::string>;
    
    using Int8 = std::int8_t;
    using UInt8 = std::uint8_t;
    
    using Int16 = std::int16_t;
    using UInt16 = std::uint16_t;
    
    using UInt = std::uint32_t;
    using Int = std::int32_t;
    using Float = float;
    using String = std::string;
    
    template <typename T>
    using Array = std::vector<T>;
    
    template <typename K, typename V>
    using Map = std::unordered_map<K, V>;
}
