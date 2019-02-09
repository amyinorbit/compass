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
        if(story_.title) {
            io_.print("\"" + *story_.title + "\"\n");
            if(story_.author) {
                io_.print("by " + *story_.author);
            }
            io_.println();
        }
        display(describeCurrent());
    }
    
    void Game::update() {
        using namespace std::placeholders;
        
        if(!run_) return;
        auto& run = run_.get();
        
        BasicEnglish grammar;
        auto phrase = io_.readLine();
        Sentence sentence(story_, phrase, grammar);
        sentence.parse()
            .flatMap(std::bind(&Game::check, this, _1))
            .flatMap(std::bind(&Game::execute, this, _1))
            .map(std::bind(&Game::display, this, _1))
            .mapError(std::bind(&Game::displayError, this, _1));
    }
    
    Result<Game::PlayerAction> Game::check(Sentence::Command cmd) {
        if(!story_.isVerb(cmd.verb)) return Error("I don't know the verb " + cmd.verb);
        return PlayerAction {
            story_.getVerb(cmd.verb),
            cmd.object
        };
    }
    
    Result<std::string> Game::execute(PlayerAction action) {
        switch(action.verb.kind) {
            case Verb::Go: return handleGo(action.object);
            case Verb::Look: return handleLook(action.object);
            case Verb::Take: return handleTake(action.object);
            case Verb::Drop: return handleDrop(action.object);
            case Verb::Inventory: return handleInventory(action.object);
            default: break;
        }
        return Error("This isn't handled yet");
    }
    
    Result<std::string> Game::handleGo(const std::string& object) {
        auto& run = maybe_guard(run_, "you must have a game run");
        const auto& links = run.current().links;
        const auto it = std::find_if(links.begin(), links.end(),
            [this,&object](const auto& link) {
                return link.direction == object
                    || link.target == story_.uniqueID(object);
            }
        );
            
        if(it == links.end())
            return Error("You can't go there");
        
        run.go(it->target);
        return describeCurrent(false);
    }
    
    Result<std::string> Game::handleLook(const std::string& object) {
        auto& run = maybe_guard(run_, "you must have a game run");
        if(!object.size())
            return describeCurrent(true);
        
        auto id = story_.uniqueID(object);
        const auto& place = run.current();
        const auto it = place.things.find(id);
        
        if(it == place.things.end())
            return Error("There's no such thing as " + object + " here.");
        
        return describe(id);
    }
    
    Result<std::string> Game::handleTake(const std::string& object) {
        auto& run = maybe_guard(run_, "you must have a game run");
        if(!object.size())
            return Error("You can't take nothing!");
        
        auto id = story_.uniqueID(object);
        const auto& place = run.current();
        const auto it = place.things.find(id);
        
        if(it == place.things.end())
            return Error("There's no such thing as " + object + " here.");
        
        run.take(id);
        return "You now have " + object + "\n"; // TODO: we should probably have an article in here.
    }
    
    Result<std::string> Game::handleDrop(const std::string& object) {
        auto& run = maybe_guard(run_, "you must have a game run");
        if(!object.size())
            return Error("You can't drop nothing!");
        
        auto id = story_.uniqueID(object);
        if(!run.has(id))
            return Error("You don't have anything like that");
        
        run.drop(id);
        return "You dropped " + object + "\n"; // TODO: we should probably have an article in here.
    }
    
    Result<std::string> Game::handleInventory(const std::string& object) {
        auto& run = maybe_guard(run_, "you must have a game run");
        return Error("This isn't implemented yet");
    }
    
    void Game::display(const std::string& text) {
        io_.println();
        io_.println(text);
        io_.print("> ");
    }
    
    std::string Game::describeCurrent(bool detailed) {
        auto& run = maybe_guard(run_, "you must have a game run");
        
        std::string text = "";
        
        const auto& place = run.current();
        
        text += "# " + story_.string(place.name) + "\n";
        text += story_.string(place.description) + "\n";
        
        if(!place.things.size() || !detailed) return text;
        
        text += "\n";
        bool single = place.things.size() == 1;
        text += single ? "There is " : "there are ";
        text += describe(place.things) + " here.";
        return text;
    }
    
    std::string Game::describe(const std::string& id, int depth) {
        auto& run = maybe_guard(run_, "you must have a game run");
        std::string text = "";
        
        const auto& thing = run.thing(id);
        if(thing.details) {
            text += story_.string(thing.details);
        } else {
            text += "Nothing special about the " + story_.string(thing.name) + ".";
        }
        
        // TODO: handle things that are on/in this.
        return text + "\n";
    }
    
    std::string Game::describe(const std::set<std::string>& things) {
        auto& run = maybe_guard(run_, "you must have a game run");
        
        std::string text = "";
        int idx = 0;
        for(auto id: things) {
            const auto& thing = run.thing(id);
            if(thing.description) {
                text += story_.string(thing.description);
            } else {
                if(thing.article)
                    text += story_.string(thing.article) + " ";
                text += story_.string(thing.name);
            }
            
            idx += 1;
            if(idx == things.size()-1)
                text += " and ";
            else if(idx != things.size())
                text += ", ";
        }
        return text;
    }
    
    void Game::displayError(Error error) {
        io_.println();
        io_.println(error.description);
        io_.println();
    }
}
