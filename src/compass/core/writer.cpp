//===--------------------------------------------------------------------------------------------===
// writer.cpp - Implementation of the 
// This source is part of the Compass Engine
//
// Created on 2019-02-23 by Amy Parent <amy@amyparent.com>
// Copyright (c) 2019 Amy Parent
// Licensed under the MIT License
// =^•.•^=
//===--------------------------------------------------------------------------------------------===
#include <cstdint>
#include <string>
#include "file.hpp"
#include <compass/core/file.hpp>

namespace Compass {
    
    static void write(std::ostream& out, std::uint8_t num) {
        char data[2];
        data[0] = static_cast<char>(Tag::U8);
        data[1] = num;
        out.write(data, 2);
    }
    
    static void write(std::ostream& out, std::uint16_t num) {
        char data[3];
        data[0] = static_cast<char>(Tag::U16);
        data[1] = (num >> 8) & 0xff;
        data[2] = (num & 0xff);
        out.write(data, 3);
    }
    
    static void write(std::ostream& out, std::uint32_t num) {
        char data[5];
        data[0] = static_cast<char>(Tag::U32);
        data[1] = (num >> 24) & 0xff;
        data[2] = (num >> 16) & 0xff;
        data[3] = (num >> 8) & 0xff;
        data[4] = (num & 0xff);
        out.write(data, 5);
    }
    
    static void write(std::ostream& out, std::uint64_t num) {
        char data[9];
        data[1] = static_cast<char>(Tag::U64);
        data[1] = (num >> 56) & 0xff;
        data[2] = (num >> 48) & 0xff;
        data[3] = (num >> 40) & 0xff;
        data[4] = (num >> 32) & 0xff;
        data[5] = (num >> 24) & 0xff;
        data[6] = (num >> 16) & 0xff;
        data[7] = (num >> 8) & 0xff;
        data[8] = num & 0xff;
        out.write(data, 9);
    }
    
    static void write(std::ostream& out, std::size_t num) {
        char data[9];
        data[1] = static_cast<char>(Tag::U64);
        data[1] = (num >> 56) & 0xff;
        data[2] = (num >> 48) & 0xff;
        data[3] = (num >> 40) & 0xff;
        data[4] = (num >> 32) & 0xff;
        data[5] = (num >> 24) & 0xff;
        data[6] = (num >> 16) & 0xff;
        data[7] = (num >> 8) & 0xff;
        data[8] = num & 0xff;
        out.write(data, 9);
    }
    
    static void write(std::ostream& out, const std::string& str) {
        const char tag = static_cast<char>(Tag::StrData);
        out.write(&tag, 1);
        write(out, str.size());
        out.write(str.data(), str.size());
    }
    
    /*
    Compass Binary Format 1.0
    
    We need to store:
        - a string pool
        - n entities <- issue here - what do we do for IDs? Not in the runtime string pool
            - links (part of entities? link pool?)
            - containment relationships
            - verb abilities
        - verbs?
    */
        
    std::ostream& operator<<(std::ostream& out, Story& writer) {
        const char signature[] = "AAPCMPS";
        out.write(signature, 7);
        
        // Version Number
        write(out, std::uint16_t{0x0001});
        
        // String Pool
        
        // Entities
        
        return out;
    }
}