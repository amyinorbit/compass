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
#include <compass/core/functional.hpp>
#include <compass/core/iomanaging.hpp>
#include <compass/core/story.hpp>
#include <compass/runtime/run.hpp>
#include <compass/runtime/sentence.hpp>

namespace Compass {
    
    class Game {
    public:
        Game(const Story& story, IOManaging& io) : story_(story), io_(io) {}
        
        void start();
        void update();
        
    private:
        struct PlayerAction {
            Verb::Kind action;
            std::string object;
        };

        result<std::string> execute(PlayerAction action);
        result<std::string> handleGo(const std::string& object);
        result<std::string> handleLook(const std::string& object);
        result<std::string> handleTake(const std::string& object);
        result<std::string> handleDrop(const std::string& object);
        result<std::string> handleInventory(const std::string& object);
        
        result<PlayerAction> check(Sentence::Command cmd);
        void display(const std::string& room);
        void displayError(const std::string& error);
        

        std::string describeCurrent(bool detailed = false);
        std::string describe(const std::string& id, int depth = 1);
        std::string describe(
            const Entity::RelationList& things, 
            Relation::Kind kind = Relation::In,
            std::string itHere = "here"
        );
        
        optional<Run>   run_;
        const Story&    story_;
        IOManaging&     io_;
    };
}
