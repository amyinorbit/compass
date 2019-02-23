//===--------------------------------------------------------------------------------------------===
// writer.cpp - Implementation of the 
// This source is part of the Compass Engine
//
// Created on 2019-02-23 by Amy Parent <amy@amyparent.com>
// Copyright (c) 2019 Amy Parent
// Licensed under the MIT License
// =^•.•^=
//===--------------------------------------------------------------------------------------------===
#include <cstdint>
#include <string>
#include "file.hpp"
#include <compass/core/file.hpp>

namespace Compass {
    
    static void write(std::ostream& out, std::uint8_t num) {
        char data[2];
        data[0] = static_cast<char>(Tag::U8);
        data[1] = num;
        out.write(data, 2);
    }
    
    static void write(std::ostream& out, std::uint16_t num) {
        char data[3];
        data[0] = static_cast<char>(Tag::U16);
        data[1] = (num >> 8) & 0xff;
        data[2] = (num & 0xff);
        out.write(data, 3);
    }
    
    static void write(std::ostream& out, std::uint32_t num) {
        char data[5];
        data[0] = static_cast<char>(Tag::U32);
        data[1] = (num >> 24) & 0xff;
        data[2] = (num >> 16) & 0xff;
        data[3] = (num >> 8) & 0xff;
        data[4] = (num & 0xff);
        out.write(data, 5);
    }
    
    static void write(std::ostream& out, std::uint64_t num) {
        char data[9];
        data[1] = static_cast<char>(Tag::U64);
        data[1] = (num >> 56) & 0xff;
        data[2] = (num >> 48) & 0xff;
        data[3] = (num >> 40) & 0xff;
        data[4] = (num >> 32) & 0xff;
        data[5] = (num >> 24) & 0xff;
        data[6] = (num >> 16) & 0xff;
        data[7] = (num >> 8) & 0xff;
        data[8] = num & 0xff;
        out.write(data, 9);
    }
    
    static void write(std::ostream& out, std::size_t num) {
        write(out, static_cast<std::uint64_t>(num));
    }
    
    static void write(std::ostream& out, const std::string& str) {
        const char tag = static_cast<char>(Tag::StrData);
        out.write(&tag, 1);
        write(out, str.size());
        out.write(str.data(), str.size());
    }
    
    /*
    Compass Binary Format 1.0
    
    We need to store:
        - a string pool
        - n entities <- issue here - what do we do for IDs? Not in the runtime string pool
            - links (part of entities? link pool?)
            - containment relationships
            - verb abilities
        - verbs?
    */
    
    static void startSection(std::ostream& out, Section mark) {
        write(out, static_cast<std::uint8_t>(Section::Start));
        write(out, static_cast<std::uint8_t>(mark));
    }
    
    static void endSection(std::ostream& out) {
        write(out, static_cast<std::uint8_t>(Section::End));
    }
    
    template <typename T>
    static void writeEnum(std::ostream& out, Marker kind, T value) {
        write(out, static_cast<std::uint8_t>(kind));
        write(out, static_cast<std::uint8_t>(value));
    }
    
    static void writeStringID(std::ostream& out, StringID id) {
        const char tag = static_cast<char>(Tag::StrID);
        write(out, static_cast<std::uint32_t>(id));
    }
    
    static void write(std::ostream& out, const Entity& e) {
        startSection(out, Section::Entity);
        write(out, e.id);
        writeStringID(out, e.article);
        writeStringID(out, e.name);
        writeStringID(out, e.description);
        
        if(e.container) {
            startSection(out, Section::Container);
            writeEnum(out, Marker::RelationKind, e.container->kind);
            write(out, e.container->id);
            endSection(out);
        }
        
        // Write things in/on/under this
        startSection(out, Section::Things);
        for(const auto& thing: e.things) {
            writeEnum(out, Marker::RelationKind, thing.second);
            write(out, thing.first);
        }
        endSection(out);
        
        // Write actions
        startSection(out, Section::Actions);
        for(const auto& action: e.actions) {
            writeEnum(out, Marker::ActionKind, action.kind);
            write(out, action.verb);
        }
        endSection(out);
        
        // And finally write links
        startSection(out, Section::Links);
        
        endSection(out);
        for(const auto& link: e.links) {
            write(out, link.target);
            write(out, link.direction);
        }
        endSection(out);
    }
    
    void write(std::ostream& out, const Story& story) {
        const char signature[] = "AAPCMPS";
        out.write(signature, 7);
        
        std::cout << "WRITING\n";
        
        // Version Number
        write(out, std::uint16_t{0x0001});
        
        // Metadata (title, author)
        // TODO: have some sort of markers for these
        startSection(out, Section::Metadata);
        if(story.title && story.author) {
            write(out, *story.title);
            write(out, *story.author);
        }
        endSection(out);
        
        // String Pool
        startSection(out, Section::StringPool);
        const auto& pool = story.strings();
        write(out, pool.size());
        for(const auto& str: pool) write(out, str);
        endSection(out);
        
        // Directions
        startSection(out, Section::Directions);
        const auto& directions = story.directions();
        write(out, directions.size());
        for(const auto& dir: directions) write(out, dir);
        endSection(out);
        
        // Verbs
        startSection(out, Section::Verbs);
        const auto& verbs = story.verbs();
        write(out, verbs.size());
        for(const auto& p: verbs) {
            writeEnum(out, Marker::VerbKind, p.second);
            write(out, p.first);
        }
        endSection(out);
        
        // Entities
        startSection(out, Section::Entities);
        write(out, story.prototype.entities.size());
        
        for(const auto& e: story.prototype.entities) write(out, e.second);
        
        endSection(out);
    }
}