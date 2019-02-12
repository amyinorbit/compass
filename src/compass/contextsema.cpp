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
        
        void ContextSema::declare(Entity::Kind kind, const Noun& name) {
            const auto id = story_.uniqueID(name.text);
            if(entities_.find(id) != entities_.end()) {
                error("You cannot create '" + name.text + "' more than one time");
                return;
            }
            
            Entity e;
            e.id = story_.stringID(id);
            e.kind = Entity::Place;
            
            if(name.article)
                e.article = story_.stringID(*name.article);
            e.name = story_.stringID(name.text);
            entities_[id] = e;
            
            current_ = id;
        }
        
        void ContextSema::setDescription(const string& text) {
            get({}).map([this,&text](auto id) {
                
                auto& e = entities_.at(id);
                e.description = story_.stringID(text);
                
            }).map_error([this,&text](auto msg) {
                error("SET_DESCRIPTION/" + msg);
            });
        }

        void ContextSema::setProperty(optional<string> entity, Property property) {
            
        }
        
        void ContextSema::addLink(const optional<string>& from, const string& to, const string& direction) {
            get(from).map([this,&to,&direction](auto id){
                
                links_.push_back(FutureLink{id, story_.uniqueID(to), String::toLower(direction)});
                
            }).map_error([this,&to](auto msg) {
                error("ADD_LOCATION/" + msg);
            });
        }

        // TODO: add support for in/on
        void ContextSema::setContainer(const optional<string>& entity, const string& container) {
            get(entity).map([this,&container](auto id) {
                
                auto& e = entities_.at(id);
                e.location = story_.uniqueID(container);
                
            }).map_error([this,&container](auto msg) {
                error("SET_CONTAINER/" + msg);
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
            }
            if(!current_)
                return make_unexpected("I can't tell what thing or place you are talking about");
            return *current_;
        }
}
