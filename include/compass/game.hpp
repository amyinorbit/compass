//===--------------------------------------------------------------------------------------------===
// game.hpp - Manages a game
// This source is part of the Compass Engine
//
// Created on 2019-02-07 by Amy Parent <amy@amyparent.com>
// Copyright (c) 2019 Amy Parent
// Licensed under the MIT License
// =^•.•^=
//===--------------------------------------------------------------------------------------------===
#pragma once
#include <compass/utils/maybe.hpp>
#include <compass/iomanaging.hpp>
#include <compass/story.hpp>
#include <compass/run.hpp>

namespace Compass {
    
    class Game {
    public:
        Game(const Story& story, IOManaging& io) : story_(story), io_(io) {}
        
        void start();
        void update();
        
    private:
        const std::string& string(StringID id) const;
        
        Maybe<Run>      run_;
        const Story&    story_;
        IOManaging&     io_;
    };
}
