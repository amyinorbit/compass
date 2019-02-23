//===--------------------------------------------------------------------------------------------===
// path.cpp - Implementation of basic filesystem path operations
// This source is part of the Compass Engine
//
// Created on 2019-02-23 by Amy Parent <amy@amyparent.com>
// Copyright (c) 2019 Amy Parent
// Licensed under the MIT License
// =^•.•^=
//===--------------------------------------------------------------------------------------------===
#include <vector>
#include <sstream>
#include <compass/path.hpp>

namespace Compass {
    
    template <char D>
    class PathComponent : public std::string {};
    
    template <char D>
    std::istream& operator>>(std::istream& is, PathComponent<D>& output)
    {
        std::getline(is, output, D);
        return is;
    }
    
    Path& Path::operator=(const Path& other) {
        if(&other != this) {
            path_ = other.path_;
        }
        return *this;
    }
    
    Path& Path::operator+=(const Path& other) {
        // TODO: this probably isn't sufficient. What happens if you try to add /Users + /hello? 
        if(path_.size())
            path_ += Path::separator + other.path_;
        else
            path_ = other.path_;
        return *this;
    }
    
    const Path Path::operator+(const Path& other) {
        return Path(*this) += other;
    }
    
    Path Path::canonical() const {
        std::istringstream iss(path_);
        auto current = std::istream_iterator<PathComponent<separator>>(iss);
        auto end = std::istream_iterator<PathComponent<separator>>();
        
        std::vector<std::string> components;
        
        while(current != end) {
            const auto& component = *(current++);
            
            if(!component.size()) continue;
            if(component == ".") continue;
            if(component == "..") {
                if(components.size()) components.pop_back();
                continue;
            }
            
            components.push_back(component);
        }
        
        Path result;
        for(const auto& c: components)
            result += Path(c);
        return result;
    }
}
