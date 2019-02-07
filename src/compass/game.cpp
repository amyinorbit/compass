//===--------------------------------------------------------------------------------------------===
// game.cpp - Implementation of the game manager
// This source is part of the Compass Engine
//
// Created on 2019-02-07 by Amy Parent <amy@amyparent.com>
// Copyright (c) 2019 Amy Parent
// Licensed under the MIT License
// =^•.•^=
//===--------------------------------------------------------------------------------------------===
#include <compass/game.hpp>

namespace Compass {
    
    void Game::start() {
        run_ = Run(story_);
    }
    
    void Game::update() {
        if(!run_) return;
        auto& run = run_.get();
        io_.print("running update");
    }
}
