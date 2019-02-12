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
        
        void ContextSema::checkDirection(const std::string& direction) {
            if(directions_.find(direction) != directions_.end()) return;
            error("you are trying to use the direction " + direction + ", which I don't know about");
        }
        
        bool ContextSema::hasOppositeDirection(const std::string& direction) {
            const auto it = directions_.find(direction);
            assert(it != directions_.end() && "invalid direction given for opposite check");
            return it->second.opposite.has_value();
        }
        
        std::string ContextSema::oppositeDirection(const std::string& direction) {
            const auto it = directions_.find(direction);
            assert(it != directions_.end() && "invalid direction given for opposite");
            assert(it->second.opposite.has_value() && "direction has no opposite");
            return *it->second.opposite;
        }
        
        void ContextSema::declare(Entity::Kind kind, const Noun& name) {
            const auto id = story_.uniqueID(name.text);
            if(entities_.find(id) != entities_.end()) {
                error("You cannot create '" + name.text + "' more than one time");
                return;
            }
            
            if(!start_ && kind == Entity::Place) {
                start_ = id;
            }
            
            Entity e;
            e.id = id;
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
            // TODO: implementation
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
            //if(error_) return make_unexpected(error_);
            if(!start_) return make_unexpected("I need at least one room to make a story");
            
            for(const auto& link: links_) {
            
                const auto fromIt = entities_.find(link.from);
                const auto toIt = entities_.find(link.to);
                
                if(fromIt == entities_.end()) {
                    return make_unexpected(
                        "I can't make a link from " + link.from + " to " + link.to
                        + " because " + link.from + " isn't a room"
                    );
                }
                
                if(fromIt->second.kind != Entity::Place)
                    return make_unexpected(fromIt->second.id + " is a thing, not a place");
                
                if(toIt == entities_.end()) {
                    return make_unexpected(
                        "I can't make a link from " + link.from + " to " + link.to
                        + " because " + link.to + " isn't a room"
                    );
                }
                
                if(toIt->second.kind != Entity::Place)
                    return make_unexpected(fromIt->second.id + " is a thing, not a place");
                
                entities_.at(link.from).links.push_back(Link{link.to, link.direction});
                if(!hasOppositeDirection(link.direction)) continue;
                entities_.at(link.to).links.push_back(Link{link.from, oppositeDirection(link.direction)});
            }
            
            for(const auto& pair: entities_) {
                if(pair.second.kind != Entity::Thing) continue;
                
                const auto& thing = pair.second;
                
                auto it = entities_.find(*thing.location);
                if(it == entities_.end())
                    return make_unexpected("cannot put thing in a container that does not exist");
                it->second.things.insert(thing.id);
            }
            
            return make_unexpected("unimplemented");
            story_.prototype.startID = *start_;
            story_.prototype.entities = entities_;
            return story_;
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
