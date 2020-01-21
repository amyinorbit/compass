//===--------------------------------------------------------------------------------------------===
// memory.hpp - Compass runtime memory manager
//
// Created by Amy Parent <amy@amyparent.com>
// Copyright (c) 2019 Amy Parent
// Licensed under the MIT License
// =^•.•^=
//===--------------------------------------------------------------------------------------------===
#pragma once
#include <compass/types.hpp>
#include <cstddef>
#include <type_traits>

namespace amyinorbit::compass {

    class Memory {
    public:
        using size_type = u32;

        Memory(size_type capacity);
        ~Memory();

        template <typename T, std::enable_if_t<sizeof(T) < 4>* = 0>
        void write(size_type address, const T& value) {
            T* dest = reinterpret_cast<T*>(data_ + address);
            *dest = value;
        }

        template <typename T, std::enable_if_t<sizeof(T) < 4>* = 0>
        T read(size_type address) const {
            const T* dest = reinterpret_cast<const T*>(data_ + address);
            return *dest;
        }

    private:
        u8* data_;
    };
}
