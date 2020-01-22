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
        Memory(const Memory& other) = delete;
        Memory(Memory&& other);
        ~Memory();

        void* ptr(size_type address) {
            return reinterpret_cast<void*>(data_ + address);
        }

        const void* ptr(size_type address) const {
            return reinterpret_cast<const void*>(data_ + address);
        }

        template <typename T, std::enable_if_t<sizeof(T) <= 4>* = nullptr>
        T* ptr(size_type address) {
            return reinterpret_cast<T*>(data_ + address);
        }

        template <typename T, std::enable_if_t<sizeof(T) <= 4>* = nullptr>
        const T* ptr(size_type address) const {
            return reinterpret_cast<const T*>(data_ + address);
        }

        template <typename T, std::enable_if_t<sizeof(T) <= 4>* = nullptr>
        void write(size_type address, const T& value) {
            T* dest = reinterpret_cast<T*>(data_ + address);
            *dest = value;
        }

        template <typename T, std::enable_if_t<sizeof(T) <= 4>* = nullptr>
        const T& read(size_type address) const {
            const T* dest = reinterpret_cast<const T*>(data_ + address);
            return *dest;
        }

        size_type alloc(size_type slots);
        void dealloc(size_type address);

        void debug(const string& marker = "") const;

    private:

        static constexpr size_type align = 4;
        static constexpr size_type block_header_size = 3 * align;
        static constexpr size_type relax_param = 10;

        static constexpr size_type block_size_offset = 0 * align;
        static constexpr size_type block_free_offset = 1 * align;
        static constexpr size_type block_next_offset = 2 * align;

        void split(size_type block, size_type size);
        void merge();

        template <typename T> T& ref(size_type address) { return *ptr<T>(address); }
        template <typename T> const T& ref(size_type address) const { return *ptr<T>(address); }

        u8* data_ = nullptr;
        size_type capacity_ = 0;
        size_type free_list_ = 0;
    };
}
