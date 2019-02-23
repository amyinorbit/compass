//===--------------------------------------------------------------------------------------------===
// path.hpp - Filesystem helper functions
// This source is part of the Compass Engine
//
// Created on 2019-02-23 by Amy Parent <amy@amyparent.com>
// Copyright (c) 2019 Amy Parent
// Licensed under the MIT License
// =^•.•^=
//===--------------------------------------------------------------------------------------------===
#pragma once
#include <string>

namespace Compass {
namespace Filesystem {
    
    class Path {
    public:
        
#if defined(_WIN32) || defined(WIN32) 
        static const char separator = '\\';
#else
        static const char separator = '/';
#endif
        
        Path() : path_("") {}
        Path(const std::string& path) : path_(path) {}
        Path(const Path& other) : path_(other.path_) {}
        Path& operator=(const Path& other);
        
        // MARK: - Path combination
        Path& operator+=(const Path& other);
        const Path operator+(const Path& other);
        
        Path canonical() const;
        
        const std::string& get() const { return path_; }
    private:
        std::string path_;
    };
}
}
