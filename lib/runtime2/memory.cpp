//===--------------------------------------------------------------------------------------------===
// memory.cpp - Memory manager implementation for Compass
//
// Created by Amy Parent <amy@amyparent.com>
// Copyright (c) 2019 Amy Parent
// Licensed under the MIT License
// =^•.•^=
//===--------------------------------------------------------------------------------------------===
#include <compass/runtime2/memory.hpp>
#include <cassert>

namespace amyinorbit::compass {

    template <typename T, typename U>
    T* as(U* ptr) {
        return reinterpret_cast<T*>(ptr);
    }

    Memory::Memory(size_type capacity) : capacity_(capacity), free_list_(16) {
        data_ = new u8[capacity];
        write<float>(0, 123.f);

        ref<size_type>(free_list_ + block_size_offset) = capacity_ - block_header_size;
        ref<bool>(free_list_ + block_free_offset) = true;
        ref<size_type>(free_list_ + block_next_offset) = 0;
    }

    Memory::Memory(Memory&& other)
        : data_(other.data_), capacity_(other.capacity_), free_list_(other.free_list_) {
        other.data_ = nullptr;
        other.capacity_ = 0;
        other.free_list_ = 0;
    }

    Memory::~Memory() {
        delete [] data_;
        capacity_ = 0;
        free_list_ = 0;
    }

    Memory::size_type Memory::alloc(size_type slots) {
        size_type required = slots * align;
        // std::cerr << "[memory]: " << "alloc " << slots << " slots (" << required << " bytes)\n";

        size_type addr = free_list_;

        while(addr) {

            // std::cerr << "  ----    block at " << addr << "\n";
            auto is_free = ref<bool>(addr + block_free_offset);
            auto size = ref<size_type>(addr + block_size_offset);
            if(!is_free || size < required) {
                // std::cerr << "          block not available (" << (is_free ? "free" : "not free") << ", " << size << ")\n";
                addr = ref<size_type>(addr + block_next_offset);
                continue;
            }

            if(size > (required + block_header_size + relax_param)) {
                // std::cerr << "          splitting block (" << required << "/" << size << ")\n";
                split(addr, required);
            }
            // std::cerr << "          allocated block (" << addr << ":" << required << ")\n";
            return addr + block_header_size;
        }
        // std::cerr << "[memory]: no block large enough\n";
        return 0;
    }

    void Memory::debug(const string& marker) const {
        size_type addr = free_list_;

        std::cout << "\n==memdump (" << marker << ")==\n";
        while(addr) {
            auto is_free = ref<bool>(addr + block_free_offset);
            auto size = ref<size_type>(addr + block_size_offset);

            std::cout << "|-----------------\n";
            std::cout << "| addr: " << std::hex << addr << std::dec << "\n";
            std::cout << "| size: " << size << "\n";
            std::cout << "| used: " << (is_free ? "no" : "yes") << "\n";

            addr = ref<size_type>(addr + block_next_offset);
        }
        std::cout << "|-----------------\n\n";
    }

    void Memory::dealloc(size_type address) {
        assert(address < capacity_ && "invalid memory block address");
        address -= block_header_size;
        // std::cerr << "[memory]: deallocating block at " << address << "\n";
        ref<bool>(address + block_free_offset) = true;
        merge();
    }

    void Memory::merge() {
        size_type current = free_list_;

        while(current) {
            bool is_free = ref<bool>(current + block_free_offset);
            auto next = ref<size_type>(current + block_next_offset);
            if(!is_free) {
                current = next;
                continue;
            };
            bool next_is_free = ref<bool>(next + block_free_offset);
            if(!next_is_free) {
                current = next;
                continue;
            }

            // std::cerr << "[memory]: merging blocks\n";

            auto size = ref<size_type>(current + block_size_offset)
                        + ref<size_type>(next + block_size_offset)
                        + block_header_size;
            next = current + size + block_header_size;

            ref<size_type>(current + block_size_offset) = size;
            ref<size_type>(current + block_next_offset) = next;
            current = current + next;
        }
    }

    void Memory::split(size_type addr, size_type size) {
        auto current_size = ref<size_type>(addr + block_size_offset);
        auto current_next = ref<size_type>(addr + block_next_offset);
        auto new_addr = addr + 3 * align + size;

        ref<size_type>(new_addr + block_size_offset) = current_size - (size + block_header_size);
        ref<bool>(new_addr + block_free_offset) = true;
        ref<size_type>(new_addr + block_next_offset) = current_next;

        ref<size_type>(addr + block_size_offset) = size;
        ref<bool>(addr + block_free_offset) = false;
        ref<size_type>(addr + block_next_offset) = new_addr;
    }
}
