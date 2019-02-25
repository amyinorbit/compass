//===--------------------------------------------------------------------------------------------===
// loader.cpp - Implements binary story file loading
// This source is part of the Compass Engine
//
// Created on 2019-02-23 by Amy Parent <amy@amyparent.com>
// Copyright (c) 2019 Amy Parent
// Licensed under the MIT License
// =^•.•^=
//===--------------------------------------------------------------------------------------------===
#include "loader.hpp"
#include <cstdio>
namespace Compass {
    
    optional<std::uint8_t> Loader::readU8() {
        if(failed()) return {};
        char data[2];
        if(!in_.read(data, 2)) return {};
        if(static_cast<Tag>(data[0]) != Tag::U8) {};
        return static_cast<std::uint8_t>(data[1]);
    }
    
    optional<std::uint16_t> Loader::readU16() {
        if(failed()) return {};
        char data[3];
        if(!in_.read(data, 3)) return {};
        if(static_cast<Tag>(data[0]) != Tag::U16) return {};
        return (data[2] << 8) | data[1];
    }
    
    optional<std::uint32_t> Loader::readU32() {
        if(failed()) return {};
        char data[5];
        if(!in_.read(data, 5)) return {};
        if(static_cast<Tag>(data[0]) != Tag::U32) return {};
        
        uint32_t result = 0;
        for(int i = 0; i < 4; ++i)
            result |= (data[i+1] << (i*8));
        return result;
    }
    
    optional<std::uint64_t> Loader::readU64() {
        if(failed()) return {};
        char data[9];
        if(!in_.read(data, 9)) return {};
        if(static_cast<Tag>(data[0]) != Tag::U64) return {};
        
        uint32_t result = 0;
        for(int i = 0; i < 8; ++i)
            result |= ((data[i+1] & 0xff) << (i*8));
        return result;
    }
    
    optional<std::string> Loader::readString() {
        if(failed()) return {};
        char marker;
        if(!in_.get(marker)) return {};
        if(static_cast<Tag>(marker) != Tag::StrData) return {};
        
        auto length = readU64();
        if(!length) return {};
        std::string result;
        
        for(std::size_t i = 0; i < *length; ++i) {
            char c;
            if(!in_.get(c)) return {};
            result.push_back(c);
        }
        return result;
    }

    optional<StringID> Loader::readStringID() {
        if(failed()) return {};
        char marker;
        if(!in_.get(marker)) return {};
        if(static_cast<Tag>(marker) != Tag::StrID) return {};
        return readU32();
    }

    
    template <typename T>
    bool assign(T& target, const optional<T>& v) {
        if(!v) return false;
        target = *v;
        return true;
    }
    
    void Loader::recSignature() {
        char signature[7];
        if(!in_.read(signature, 7)) { error("unable to read file signature"); return; }
        if(memcmp("AAPCMPS", signature, 7) != 0) { error("invalid file signature"); return; }
        
        auto version = readU16();
        if(!version) { error("unable to read file version"); return; }
        if(*version != 0x0001) { error("invalid version number"); return; }
    }
    
    void Loader::recMetadata() {
        if(failed()) return;
        if(!match(Section::Metadata)) { error("missing metadata section"); return; }
        if(!match(Section::Start)) { error("malformed metadata section"); return; }
        
        
        auto title = readString();
        if(!title) { error("can't read title"); return; }
        story_.title.emplace(*title);
        auto author = readString();
        if(!author) { error("can't read title"); return; }
        story_.author.emplace(*author);
        if(!assign(story_.prototype.startID, readString())) { error("missing start room ID"); return; }
        
        if(!match(Section::End)) { error("malformed metadata section"); return; }
    }
    
    void Loader::recStringPool() {
        if(failed()) return;
        if(!match(Section::StringPool)) { error("missing string pool section"); return; }
        if(!match(Section::Start)) { error("malformed string pool section"); return; }
        
        auto count = readU64();
        if(!count) { error("unable to read string pool size"); return; }
        
        for(std::size_t i = 0; i < *count; ++i) {
            auto string = readString();
            if(!string) { error("unable to read string"); return; }
            story_.intern(*string);
        }
        
        if(!match(Section::End)) { error("malformed string pool section"); return; }
    }
    
    void Loader::recDirections() {
        if(failed()) return;
        if(!match(Section::Directions)) { error("missing directions section"); return; }
        if(!match(Section::Start)) { error("malformed directions section"); return; }
        
        auto count = readU64();
        if(!count) { error("unable to read directions table size"); return; }
        
        for(std::size_t i = 0; i < *count; ++i) {
            auto string = readString();
            if(!string) { error("unable to read string"); return; }
            story_.directions_.insert(*string);
        }
        
        if(!match(Section::End)) { error("malformed directions section"); return; }
    }
    
    void Loader::recVerbs() {
        if(failed()) return;
        if(!match(Section::Verbs)) { error("missing verbs section"); return; }
        if(!match(Section::Start)) { error("malformed verbs section"); return; }
        
        auto count = readU64();
        if(!count) { error("unable to read verbs table size"); return; }
        
        for(std::size_t i = 0; i < *count; ++i) {
            if(!match(Marker::VerbKind)) { error("not a valid verb"); return; }
            auto kind = readEnum<Verb::Kind>();
            if(!kind) { error("Unable to read verb type"); return; }
            
            auto verb = readString();
            if(!verb) { error("unable to read verb"); return; }
            story_.verbs_[*verb] = *kind;
        }
        
        if(!match(Section::End)) { error("malformed verbs section"); return; }
    }
    
    void Loader::recEntities() {
        if(failed()) return;
        
        if(!match(Section::Entities)) { error("missing entities section"); return; }
        if(!match(Section::Start)) { error("malformed entities section"); return; }
        
        auto count = readU64();
        
        for(std::size_t i = 0; i < *count; ++i) {
            recEntity().map([this](const auto& entity) {
                story_.prototype.entities[entity.id] = entity;
            }).map_error([this](const auto& msg) {
                error(msg);
            });
            if(failed()) return;
        }
        
        if(!match(Section::End)) { error("malformed entities section"); return; }
    }
    
    result<Entity> Loader::recEntity() {
        Entity e;
        
        if(match(Section::Place))
            e.kind = Entity::Place;
        else if(match(Section::Thing))
            e.kind = Entity::Thing;
        else
            return make_unexpected("missing entity record");
        
        if(!match(Section::Start)) return make_unexpected("malformed entity record");
        
        if(!assign(e.id, readString())) return make_unexpected("unable to read id");
        if(!assign(e.article, readStringID()))  return make_unexpected("unable to read article");
        if(!assign(e.name, readStringID())) return make_unexpected("unable to read name");
        if(!assign(e.description, readStringID())) return make_unexpected("unable to read description");
        
        // Read containment relation
        if(have(Section::Container)) {
            auto container = recEntityContainer();
            if(!container) return make_unexpected(container.error());
            e.container = *container;
        } else {
            e.container = {};
        }
        
        // Read things
        {
            if(!match(Section::Things)) return make_unexpected("missing things table");
            if(!match(Section::Start)) return make_unexpected("malformed things table");
        
            auto count = readU64();
            if(!count) return make_unexpected("Invalid things count");
        
            for(std::size_t i = 0; i < count; ++i) {
                if(!recEntityThing(e)) return make_unexpected("malformed thing record");
            }
            if(!match(Section::End)) return make_unexpected("malformed things table");
        }
        
        // Read actions
        {
            if(!match(Section::Actions)) return make_unexpected("missing actions table");
            if(!match(Section::Start)) return make_unexpected("malformed actions table");
        
            auto count = readU64();
            if(!count) return make_unexpected("Invalid actions count");
        
            for(std::size_t i = 0; i < count; ++i) {
                auto action = recEntityAction();
                if(!action) return make_unexpected(action.error());
                e.actions.push_back(*action);
            }

            if(!match(Section::End)) return make_unexpected("malformed actions table");
            std::cout << "[read in: " << e.id << "]\n";
        }
        
        // Read links
        {
            if(!match(Section::Links)) return make_unexpected("missing links table");
            if(!match(Section::Start)) return make_unexpected("malformed links table");
        
            auto count = readU64();
            if(!count) return make_unexpected("Invalid links count");
        
            for(std::size_t i = 0; i < count; ++i) {
                auto link = recEntityLink();
                if(!link) return make_unexpected(link.error());
                e.links.push_back(*link);
            }

            if(!match(Section::End)) return make_unexpected("malformed links table");
        }

        if(!match(Section::End)) return make_unexpected("malformed entity record");
        
        return e;
    }

    result<Relation> Loader::recEntityContainer() {
        Relation relation;
        if(!match(Section::Container)) return make_unexpected("missing entity container");
        if(!match(Section::Start)) return make_unexpected("malformed entity container");
        if(!match(Marker::RelationKind)) return make_unexpected("invalid container");
        
        if(!assign(relation.kind, readEnum<Relation::Kind>())) return make_unexpected("invalid container");
        if(!assign(relation.id, readString())) return make_unexpected("invalid container id");
        
        if(!match(Section::End)) return make_unexpected("malformed entity container");
        return relation;
    }

    result<Action> Loader::recEntityAction() {
        Action action;
        if(!match(Marker::ActionKind)) return make_unexpected("invalid action");
        if(!assign(action.kind, readEnum<Action::Kind>())) return make_unexpected("invalid action");
        if(!assign(action.verb, readString())) return make_unexpected("invalid action verb");
        return action;
    }
    
    bool Loader::recEntityThing(Entity& e) {
        if(!match(Marker::RelationKind)) return false;
        Relation::Kind kind;
        std::string id;
        if(!assign(kind, readEnum<Relation::Kind>())) return false;
        if(!assign(id, readString())) return false;
        e.things[id] = kind;
        return true;
    }
    
    result<Link> Loader::recEntityLink() {
        Link link;
        if(!assign(link.target, readString())) return make_unexpected("invalid link target");
        if(!assign(link.direction, readString())) return make_unexpected("invalid action direction");
        return link;
    }
    
    result<Story> Loader::load() {
        recSignature();
        recMetadata();
        recStringPool();
        recDirections();
        recVerbs();
        recEntities();
        
        if(error_) return make_unexpected(*error_);
        return story_;
    }
    
    result<Story> load(std::istream& in) {
        return Loader(in).load();
    }
}
