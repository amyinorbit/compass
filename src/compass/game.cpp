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
        run_.emplace(story_);
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
        auto& run = *run_;
        
        BasicEnglish grammar;
        auto phrase = io_.readLine();
        Sentence(story_, phrase, grammar).parse()
            .and_then(std::bind(&Game::check, this, _1))
            .and_then(std::bind(&Game::execute, this, _1))
            .map(std::bind(&Game::display, this, _1))
            .map_error(std::bind(&Game::displayError, this, _1));
    }
    
    result<Game::PlayerAction> Game::check(Sentence::Command cmd) {
        auto action = story_.verb(cmd.verb);
        if(!action)
            return make_unexpected("I don't know the verb " + cmd.verb);
        return PlayerAction { *action, cmd.object };
    }
    
    result<std::string> Game::execute(PlayerAction action) {
        switch(action.action) {
            case Verb::Go: return handleGo(action.object);
            case Verb::Look: return handleLook(action.object);
            case Verb::Take: return handleTake(action.object);
            case Verb::Drop: return handleDrop(action.object);
            case Verb::Inventory: return handleInventory(action.object);
            default: break;
        }
        return make_unexpected("This isn't handled yet");
    }
    
    result<std::string> Game::handleGo(const std::string& object) {
        auto& run = maybe_guard(run_, "you must have a game run");
        const auto& links = run.current().links;
        const auto it = std::find_if(links.begin(), links.end(),
            [this,&object](const auto& link) {
                return link.direction == object
                    || link.target == story_.uniqueID(object);
            }
        );
            
        if(it == links.end())
            return make_unexpected("You can't go there");
        
        run.go(it->target);
        return describeCurrent(false);
    }
    
    result<std::string> Game::handleLook(const std::string& object) {
        auto& run = maybe_guard(run_, "you must have a game run");
        if(!object.size())
            return describeCurrent(true);
        
        auto id = story_.uniqueID(object);
        const auto& place = run.current();
        const auto it = place.things.find(id);
        
        if(it == place.things.end())
            return make_unexpected("There's no such thing as " + object + " here.");
        
        return describe(id);
    }
    
    result<std::string> Game::handleTake(const std::string& object) {
        auto& run = maybe_guard(run_, "you must have a game run");
        if(!object.size())
            return make_unexpected("You can't take nothing!");
        
        auto id = story_.uniqueID(object);
        const auto& place = run.current();
        const auto it = place.things.find(id);
        
        if(it == place.things.end())
            return make_unexpected("There's no such thing as " + object + " here.");
        
        const auto& thing = run.entity(id);
        
        // TODO: find something better here.
        const auto actIt = std::find_if(thing.actions.begin(), thing.actions.end(), [](const auto& a) {
            std::cout << a.verb << std::endl;
            return a.verb == "take";
        });
        if(actIt == thing.actions.end())
            return make_unexpected("you can't take that");
        
        run.take(id);
        return "You now have " + object + "\n"; // TODO: we should probably have an article in here.
    }
    
    result<std::string> Game::handleDrop(const std::string& object) {
        auto& run = maybe_guard(run_, "you must have a game run");
        if(!object.size())
            return make_unexpected("You can't drop nothing!");
        
        auto id = story_.uniqueID(object);
        if(!run.has(id))
            return make_unexpected("You don't have anything like that");
        
        run.drop(id);
        return "You dropped " + object + "\n"; // TODO: we should probably have an article in here.
    }
    
    result<std::string> Game::handleInventory(const std::string& object) {
        auto& run = maybe_guard(run_, "you must have a game run");
        return tl::make_unexpected("This isn't implemented yet");
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
        
        const auto& thing = run.entity(id);
        if(thing.description) {
            text += story_.string(thing.description);
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
            const auto& thing = run.entity(id);
            if(thing.article)
                text += story_.string(thing.article) + " ";
            text += story_.string(thing.name);
            
            idx += 1;
            if(idx == things.size()-1)
                text += " and ";
            else if(idx != things.size())
                text += ", ";
        }
        return text;
    }
    
    void Game::displayError(const std::string& error) {
        io_.println();
        io_.println(error);
        io_.println();
        io_.print("> ");
    }
}
