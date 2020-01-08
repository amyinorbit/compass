//===--------------------------------------------------------------------------------------------===
// Buffer.hpp - Compass 2.0 Runtime fixed-size array
//
// Created by Amy Parent <amy@amyparent.com>
// Copyright (c) 2019 Amy Parent
// Licensed under the MIT License
// =^•.•^=
//===--------------------------------------------------------------------------------------------===
#pragma once
#include <compass/runtime2/value.hpp>
#include <algorithm>

namespace Compass::rt2 {

    template <typename T>
    class buffer {
    public:
        using size_type = u16;
        using value_type = T;
        using reference = T&;
        using const_reference = const T&;
        using iterator = T*;
        using const_iterator = const T*;


        buffer(u16 capacity) {
            storage_ = new value_type [capacity];
            if(storage_) capacity_ = capacity;
        }
        ~buffer() {
            delete [] storage_;
            capacity_ = size_ = 0;
        }

        // TODO: implement these
        buffer(const buffer&) = delete;
        buffer(buffer&&) = delete;

        // MARK: - iterator support;
        iterator begin() { return storage_; }
        const_iterator begin() const { return storage_; }

        iterator end() { return storage_ + size_; }
        const_iterator end() const { return storage_ + size_; }

        // MARK: - Data access
        reference front() { return storage_[0]; }
        const_reference front() const { return storage_[0]; }
        reference back() { return storage_[size_-1]; }
        const_reference back() const { return storage_[size_-1]; }

        reference operator[](size_type idx) { return storage_[idx]; }
        const_reference operator[](size_type idx) const { return storage_[idx]; }

        size_type size() const { return size_; }
        size_type capacity() const { return capacity_; }

        template <typename U>
        const U& back() const { return back().template as<U>(); }

        template <typename U>
        U& back() { return back().template as<U>(); }

        // MARK: - Insertions and deletions

        void push_back(const_reference item) { storage_[size_++] = item; }
        value_type pop_back() { return storage_[--size_]; }

        template <typename U>
        U pop_back() { return pop_back().template as<U>(); }

        template <typename U>
        void push_back(const U& item) { storage_[size_++] = item; }

        template <typename U>
        void insert(iterator where, const U& item) {
            std::copy_backward(where, end(), end() + 1);
            storage_[where] = item;
            size_ += 1;
        }

        void erase(iterator where) {
            erase(where, where+1);
        }

        void erase(iterator from, iterator to) {
            std::copy(to, end(), from);
            size_ -= (to - from);
        }

    private:
        size_type size_ = 0;
        size_type capacity_ = 0;
        value_type* storage_ = nullptr;
    };
}
