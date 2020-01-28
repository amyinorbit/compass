//===--------------------------------------------------------------------------------------------===
// bin_io.hpp - Binary input/output utility classes
//
// Created by Amy Parent <amy@amyparent.com>
// Copyright (c) 2019 Amy Parent
// Licensed under the MIT License
// =^•.•^=
//===--------------------------------------------------------------------------------------------===
#pragma once
#include <compass/types.hpp>
#include <apfun/maybe.hpp>
#include <apfun/string.hpp>
#include <iostream>
#include <stdexcept>

namespace amyinorbit::compass {

    enum class Tag : u8 {
        data_object = 0xa0,
        data_list = 0xa1,
        data_utf8 = 0xa2,

        value_int = 0xaa,
        value_float  = 0xab,
        ref_string = 0xac,
        ref_object = 0xad,
        ref_list = 0xae,
        ref_nil = 0xaf,
    };

    class BinaryWriter {
    public:
        BinaryWriter(std::ostream& stream) : stream_(stream) {
            stream_.imbue(std::locale::classic());
            stream_.exceptions(std::ostream::failbit | std::ostream::badbit);
        }

        template <typename T, std::enable_if_t<sizeof(T) == 4>* = nullptr>
        void write(T value) {
            write_32(*reinterpret_cast<u32*>(&value));
        }

        template <typename T, std::enable_if_t<sizeof(T) == 2>* = nullptr>
        void write(T value) {
            write_16(*reinterpret_cast<u16*>(&value));
        }

        template <typename T, std::enable_if_t<sizeof(T) == 1>* = nullptr>
        void write(T value) {
            write_8(*reinterpret_cast<u8*>(&value));
        }

        void write(const string& string) {
            write((u32)string.size()+1);
            write(string.data(), string.size());
        }

        bool write(const char* data, u32 count) {
            return stream_.write(data, count).fail();
        }

        u64 size() const {
            auto current = stream_.tellp();
            stream_.seekp(0, std::ostream::end);
            auto size = stream_.tellp();
            stream_.seekp(current);
            return size;
        }

        void forward(u64 distance) {
            stream_.seekp(distance, std::ostream::seekdir::cur);
        }

        void backward(u64 distance) {
            stream_.seekp(-distance, std::ostream::seekdir::cur);
        }

        void go(u64 position) {
            stream_.seekp(position);
        }

    private:

        void write_8(u8 data) {
            stream_.put(data);
        }

        void write_16(u16 data) {
            stream_.put(data & 0xff);
            stream_.put((data >> 8) & 0xff);
        }

        bool write_32(u32 data) {
            stream_.put(data & 0xff);
            stream_.put((data >> 8) & 0xff);
            stream_.put((data >> 16) & 0xff);
            stream_.put((data >> 24) & 0xff);
            return !stream_.fail();
        }

        std::ostream& stream_;
    };

    class BinaryReader {
    public:
        BinaryReader(std::istream& stream) : stream_(stream) {
            stream_.exceptions(std::istream::failbit | std::istream::badbit);
        }

        template <typename T, std::enable_if_t<sizeof(T) == 4>* = nullptr>
        T read() {
            u32 data = read_32();
            return *reinterpret_cast<const T*>(&data);
        }

        template <typename T, std::enable_if_t<sizeof(T) == 2>* = nullptr>
        T read() {
            u16 data = read_16();
            return *reinterpret_cast<const T*>(&data);
        }

        template <typename T, std::enable_if_t<sizeof(T) == 1>* = nullptr>
        T read() {
            u8 data = read_8();
            return *reinterpret_cast<const T*>(&data);
        }

        string read_string() {
            auto size = read<u32>();
            string str;
            str.reserve(size + 1);

            while(size--) {
                char c;
                stream_.get(c);
                str += c;
            }
            return str;;
        }

        u8 read_8() {
            u8 data;
            stream_.read((char*)&data, 1);
            return data;
        }

        u16 read_16() {
            u8 data[2];
            stream_.read(reinterpret_cast<char*>(data), 2);
            return data[0] | (data[1] << 8);
        }

        u32 read_32() {
            u8 data[4];
            stream_.read(reinterpret_cast<char*>(data), 4);
            return data[0] | (data[1] << 8) | (data[2] << 16) | (data[3] << 24);
        }

        u64 size() const {
            auto current = stream_.tellg();
            stream_.seekg(0, std::istream::end);
            auto size = stream_.tellg();
            stream_.seekg(current);
            return size;
        }

        void forward(u64 distance) {
            stream_.seekg(distance, std::ostream::seekdir::cur);
        }

        void backward(u64 distance) {
            stream_.seekg(-distance, std::ostream::seekdir::cur);
        }

        auto go(u64 position) {
            stream_.seekg(position);
        }

    private:

        std::istream& stream_;
    };
}
