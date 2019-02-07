//===--------------------------------------------------------------------------------------------===
// game.cpp - Implementation of the game manager
// This source is part of the Compass Engine
//
// Created on 2019-02-07 by Amy Parent <amy@amyparent.com>
// Copyright (c) 2019 Amy Parent
// Licensed under the MIT License
// =^•.•^=
//===--------------------------------------------------------------------------------------------===
#include <algorithm>
#include <compass/game.hpp>
#include <compass/grammar.hpp>

namespace Compass {
    
    void Game::start() {
        run_ = Run(story_);
        displayCurrent();
    }
    
    void Game::update() {
        using namespace std::placeholders;
        
        if(!run_) return;
        auto& run = run_.get();
        
        BasicEnglish grammar;
        
        io_.print("> ");
        auto phrase = io_.readLine();
        Sentence sentence(story_, phrase, grammar);
        sentence.parse()
            .flatMap<PlayerAction>(std::bind(&Game::check, this, _1))
            .flatMap<StringID>(std::bind(&Game::execute, this, _1))
            .map(std::bind(&Game::displayResult, this, _1))
            .mapError(std::bind(&Game::displayError, this, _1));
    }
    
    Result<Game::PlayerAction> Game::check(Sentence::Command cmd) {
        if(!story_.isVerb(cmd.verb)) return Error("I don't know the verb " + cmd.verb);
        return PlayerAction {
            story_.getVerb(cmd.verb),
            cmd.object
        };
    }
    
    Result<StringID> Game::execute(PlayerAction action) {
        switch(action.verb.kind) {
            case Verb::Go: return handleGo(action.object);
            case Verb::Look: return handleLook(action.object);
            case Verb::Take: return handleTake(action.object);
            case Verb::Drop: return handleDrop(action.object);
            default: break;
        }
        return Error("This isn't handled yet");
    }
    
    Result<StringID> Game::handleGo(const std::string& object) {
        
        const auto& links = run_.get().current().links;
        const auto it = std::find_if(links.begin(), links.end(),
            [this,&object](const auto& link) {
                return link.direction == object
                    || link.target == story_.uniqueID(object);
            }
        );
            
        if(it != links.end())
            return it->target;
        return Error("You can't go there");
    }
    
    Result<StringID> Game::handleLook(const std::string& object) {
        
        return Error("You can't look at that");
    }
    
    Result<StringID> Game::handleTake(const std::string& object) {
        
        return Error("You can't take this");
    }
    
    Result<StringID> Game::handleDrop(const std::string& object) {
        return Error("You can't drop that");
    }
    
    void Game::displayResult(StringID roomID) {
        run_.get().go(roomID);
        displayCurrent();
    }
    
    void Game::displayCurrent() {
        if(!run_) return;
        auto& run = run_.get();
        
        const auto& place = run.current();
        io_.println();
        io_.println("# " + story_.string(place.name));
        io_.println();
        io_.println(story_.string(place.description));
        io_.println();
        
        if(place.things.size()) {
            bool single = place.things.size() == 1;
            io_.print(single ? "There is " : "there are ");
            
            int idx = 0;
            for(auto id: place.things) {
                const auto& thing = run.thing(id);
                io_.print(story_.string(thing.article) + " " + story_.string(thing.name));
                idx += 1;
                
                if(idx == place.things.size())
                    io_.println(" here.");
                else if(idx == place.things.size()-1)
                    io_.print(" and ");
                else
                    io_.print(", ");
            }
            io_.println();
        }
    }
    
    void Game::displayError(Error error) {
        io_.println(error.description);
        io_.println();
    }
}
