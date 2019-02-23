//===--------------------------------------------------------------------------------------------===
// file.hpp - API used to read and write storiy binary files to and from disk.
// This source is part of the Compass Engine
//
// Created on 2019-02-23 by Amy Parent <amy@amyparent.com>
// Copyright (c) 2019 Amy Parent
// Licensed under the MIT License
// =^•.•^=
//===--------------------------------------------------------------------------------------------===
#pragma once
#include <iostream>
#include <compass/core/story.hpp>

namespace Compass {
    
    // struct StoryWriter {
    //     StoryWriter(const Story& story) : story_(story) {}
    //     friend std::ostream& operator<<(std::ostream& out, StoryWriter& writer);
    // private:
    //     const Story& story_;
    // };
    
    void write(std::ostream& out, const Story& story);
}

