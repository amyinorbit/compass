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

        Result<std::string> execute(PlayerAction action);
        Result<std::string> handleGo(const std::string& object);
        Result<std::string> handleLook(const std::string& object);
        Result<std::string> handleTake(const std::string& object);
        Result<std::string> handleDrop(const std::string& object);
        
        Result<PlayerAction> check(Sentence::Command cmd);
        void display(const std::string& room);
        void displayError(Error error);
        

        std::string describeCurrent(bool detailed = false);
        std::string describe(const std::string& id, int depth = 1);
        
        Maybe<Run>      run_;
        const Story&    story_;
        IOManaging&     io_;
    };
}
