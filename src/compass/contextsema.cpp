//===--------------------------------------------------------------------------------------------===
// contextsema.cpp - Implementation of the contextual english semantics checker.
// This source is part of the Compass Engine
//
// Created on 2019-02-12 by Amy Parent <amy@amyparent.com>
// Copyright (c) 2019 Amy Parent
// Licensed under the MIT License
// =^•.•^=
//===--------------------------------------------------------------------------------------------===
#include <compass/contextsema.hpp>
#include <compass/utils/string.hpp>

namespace Compass {
    
        void ContextSema::declareDirection(const string& direction, optional<string> opposite) {
            directions_[direction] = Direction{direction, opposite};
            if(opposite) {
                directions_[*opposite] = Direction{*opposite, direction};
            }
        }
        
        void ContextSema::declarePlace(const Noun& name) {
            const auto id = story_.uniqueID(name.text);
            if(entities_.find(id) != entities_.end()) {
                error("You cannot create '" + name.text + "' more than one time");
                return;
            }
            
            Entity place;
            
            place.id = story_.stringID(id);
            place.kind = Entity::Place;
            
            if(name.article)
                place.article = story_.stringID(*name.article);
            place.name = story_.stringID(name.text);
            
            entities_[id] = place;
        }
        
        void ContextSema::declare(const Noun& name, optional<Noun> what) {
            const auto id = story_.uniqueID(name.text);
            if(entities_.find(id) != entities_.end()) {
                error("You cannot create '" + name.text + "' more than one time");
                return;
            }
            
            Entity thing;
            
            thing.id = story_.stringID(id);
            thing.kind = Entity::Unknown;
            
            if(name.article)
                thing.article = story_.stringID(*name.article);
            thing.name = story_.stringID(name.text);
            
            entities_[id] = thing;
            
            if(what) setKind(name.text, what->text);
        }
        
        void ContextSema::setKind(optional<string> entity, Entity::Kind what) {
            get(entity).and_then([this,what](auto id) -> result<Entity::Kind> {
                
                auto& e = entities_.at(id);
                if(e.kind != Entity::Unknown)
                    return make_unexpected(story_.string(e.name) + " cannot be multiple things");
                e.kind = what;
                return what;
                
            }).map_error([this](auto msg) { error(msg); });
        }
        
        void ContextSema::setKind(optional<string> entity, const string& what) {
            
            get(entity).and_then([this,&what](auto id) -> result<Entity::Kind> {
                
                auto kind = String::toLower(what);
                auto& e = entities_.at(id);
                if(e.kind != Entity::Unknown)
                    return make_unexpected(story_.string(e.name) + " cannot be multiple things");
                return e.kind = (kind == "place" || kind == "room")
                              ? Entity::Place
                              : Entity::Thing;
                
            }).map_error([this](auto msg) { error(msg); });
        }
        
        void ContextSema::setDescription(const string& text) {
            get({}).map([this,&text](auto id) {
                
                auto& e = entities_.at(id);
                e.description = story_.stringID(text);
                
            }).map_error([this](auto msg) {
                error(msg);
            });
        }

        void ContextSema::setProperty(optional<string> entity, Property property) {
            
        }
        
        void ContextSema::addLocation(optional<string> from, const string& to, const string& direction) {
            
        }
        
        void ContextSema::addAbility(optional<string> entity, const string& verb) {
            
        }

        // TODO: add support for in/on
        void ContextSema::setContainer(optional<string> entity, const string& container) {
            get(entity).map([this,&container](auto id) {
                
                auto& e = entities_.at(id);
                e.location = story_.uniqueID(container);
                
            }).map_error([this](auto msg) {
                error(msg);
            });
        }

        result<Story> ContextSema::resolve() {
            return make_unexpected("unimplemented");
        }
        
        void ContextSema::error(const string& message) {
            std::cerr << "[SEMA]: " << message << "\n";
        }
        
        result<ContextSema::string> ContextSema::get(optional<string> entity) {
            if(entity) {
                auto id = story_.uniqueID(*entity);
                current_ = *entity;
                std::cout << "current: " << *entity << "\n";
            }
            if(!current_)
                return make_unexpected("I can't tell what thing or place you are talking about");
            return *current_;
        }
}
