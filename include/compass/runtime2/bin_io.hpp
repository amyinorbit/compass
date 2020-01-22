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

    struct IOException : public std::runtime_error {
        using std::runtime_error::runtime_error;
    };

    class BinaryWriter {
    public:
        BinaryWriter(std::ostream& stream) : stream_(stream) {
            stream_.imbue(std::locale::classic());
        }

        template <typename T>
        bool write(T value) {
            return write(*reinterpret_cast<u32*>(&value));
        }

        bool write(const char* data, u32 count) {
            return stream_.write(data, count).fail();
        }

        bool write(const string& string) {
            if(!write((u32)string.size())) return false;
            return write(string.data(), string.size());
        }

    private:

        bool write(u32 data) {
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
        BinaryReader(std::istream& stream) : stream_(stream) { }

        template <typename T>
        maybe<T> read() {
            return read_raw() >> [](u32 data) {
                return *reinterpret_cast<const T*>(&data);
            };
        }

        maybe<string> read_string() {
            return read_raw() >= [this](u32 size) -> maybe<string> {
                string str;
                str.reserve(size + 1);

                while(size--) {
                    char c;
                    if(!stream_.get(c)) return nothing();
                    str += c;
                }
                return str;
            };
        }

    private:

        maybe<u32> read_raw() {
            u8 data[4];
            if(!stream_.read(reinterpret_cast<char*>(data), 4)) return nothing();
            return data[0] | (data[1] << 8) | (data[2] << 16) | (data[3] << 24);
        }

        std::istream& stream_;
    };
}
