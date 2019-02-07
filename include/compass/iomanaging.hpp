//===--------------------------------------------------------------------------------------------===
// iomanaging.hpp - 
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

namespace Compass {
    
    class IOManaging {
    public:
        virtual ~IOManaging() {}
        
        virtual void print(const std::string& str) = 0;
        virtual void println(const std::string& str = "") = 0;
        virtual void clear() = 0;
        virtual std::string readLine() = 0;
    };
    
    class StreamIO: public IOManaging {
    public:
        virtual ~StreamIO() {}
        
        virtual void print(const std::string& str) {
            std::cout << str;
        }
        
        virtual void println(const std::string& str = "") {
            std::cout << str << "\n";
        }
        
        virtual void clear() {
            std::cout << "\n\n\n\n\n";
        }
        
        virtual std::string readLine() {
            std::string line;
            std::getline(std::cin, line);
            return line;
        }
    };
    
}
