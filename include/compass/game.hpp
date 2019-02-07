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
#include <compass/utils/result.hpp>
#include <compass/utils/maybe.hpp>
#include <compass/iomanaging.hpp>
#include <compass/sentence.hpp>
#include <compass/story.hpp>
#include <compass/run.hpp>

namespace Compass {
    
    class Game {
    public:
        Game(const Story& story, IOManaging& io) : story_(story), io_(io) {}
        
        void start();
        void update();
        
    private:
        struct PlayerAction {
            Verb verb;
            std::string object;
        };
        
        Result<StringID> handleGo(const std::string& object);
        Result<StringID> handleLook(const std::string& object);
        Result<StringID> handleTake(const std::string& object);
        Result<StringID> handleDrop(const std::string& object);
        
        Result<PlayerAction> check(Sentence::Command cmd);
        Result<StringID> execute(PlayerAction action);
        void displayResult(StringID room);
        void displayCurrent();
        void displayError(Error error);
        
        Maybe<Run>      run_;
        const Story&    story_;
        IOManaging&     io_;
    };
}
