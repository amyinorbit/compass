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
    
    static void writeU8(std::ostream& out, std::uint8_t num) {
        char data[2];
        data[0] = static_cast<char>(Tag::U8);
        data[1] = num;
        out.write(data, 2);
    }
    
    static void writeU16(std::ostream& out, std::uint16_t num) {
        char data[3];
        data[0] = static_cast<char>(Tag::U16);
        data[1] = (num & 0xff);
        data[2] = (num >> 8) & 0xff;
        out.write(data, 3);
    }
    
    static void writeU32(std::ostream& out, std::uint32_t num) {
        char data[5];
        data[0] = static_cast<char>(Tag::U32);
        data[1] = (num & 0xff);
        data[2] = (num >> 8) & 0xff;
        data[3] = (num >> 16) & 0xff;
        data[4] = (num >> 24) & 0xff;
        out.write(data, 5);
    }
    
    static void writeU64(std::ostream& out, std::uint64_t num) {
        char data[9];
        data[0] = static_cast<char>(Tag::U64);
        data[1] = num & 0xff;
        data[2] = (num >> 8) & 0xff;
        data[3] = (num >> 16) & 0xff;
        data[4] = (num >> 24) & 0xff;
        data[5] = (num >> 32) & 0xff;
        data[6] = (num >> 40) & 0xff;
        data[7] = (num >> 48) & 0xff;
        data[8] = (num >> 56) & 0xff;
        out.write(data, 9);
    }
    
    static void writeSize(std::ostream& out, std::size_t num) {
        writeU64(out, static_cast<std::uint64_t>(num));
    }
    
    static void writeString(std::ostream& out, const std::string& str) {
        const char tag = static_cast<char>(Tag::StrData);
        out.write(&tag, 1);
        writeSize(out, str.size());
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
        char data[2];
        data[0] = static_cast<char>(mark);
        data[1] = static_cast<char>(Section::Start);
        out.write(data, 2);
    }
    
    static void endSection(std::ostream& out) {
        char data = static_cast<std::uint8_t>(Section::End);
        out.write(&data, 1);
    }
    
    template <typename T>
    static void writeEnum(std::ostream& out, Marker kind, T value) {
        char data[2];
        data[0] = static_cast<std::uint8_t>(kind);
        data[1] = static_cast<std::uint8_t>(value);
        out.write(data, 2);
    }
    
    static void writeStringID(std::ostream& out, StringID id) {
        char tag = static_cast<char>(Tag::StrID);
        out.write(&tag, 1);
        writeU32(out, static_cast<std::uint32_t>(id));
    }
    
    static void write(std::ostream& out, const Entity& e) {
        switch(e.kind) {
            case Entity::Thing: startSection(out, Section::Thing); break;
            case Entity::Place: startSection(out, Section::Place); break;
        }
        
        writeString(out, e.id);
        writeStringID(out, e.article);
        writeStringID(out, e.name);
        writeStringID(out, e.description);
        
        if(e.container) {
            startSection(out, Section::Container);
            writeEnum(out, Marker::RelationKind, e.container->kind);
            writeString(out, e.container->id);
            endSection(out);
        }
        
        // Write things in/on/under this
        startSection(out, Section::Things);
        writeSize(out, e.things.size());
        for(const auto& thing: e.things) {
            writeEnum(out, Marker::RelationKind, thing.second);
            writeString(out, thing.first);
        }
        endSection(out);
        
        // Write actions
        startSection(out, Section::Actions);
        writeSize(out, e.actions.size());
        for(const auto& action: e.actions) {
            writeEnum(out, Marker::ActionKind, action.kind);
            writeString(out, action.verb);
        }
        endSection(out);
        
        // And finally write links
        startSection(out, Section::Links);
        writeSize(out, e.links.size());
        for(const auto& link: e.links) {
            writeString(out, link.target);
            writeString(out, link.direction);
        }
        endSection(out);
        
        endSection(out);
    }
    
    void write(std::ostream& out, const Story& story) {
        const char signature[] = "AAPCMPS";
        out.write(signature, 7);
        
        // Version Number
        writeU16(out, std::uint16_t{0x0001});
        
        // Metadata (title, author)
        // TODO: have some sort of markers for these
        startSection(out, Section::Metadata);
        writeString(out, *story.title.disjunction(""));
        writeString(out, *story.author.disjunction(""));
        writeString(out, story.prototype.startID);
        endSection(out);
        
        // String Pool
        startSection(out, Section::StringPool);
        const auto& pool = story.strings();
        writeSize(out, pool.size());
        for(const auto& str: pool) writeString(out, str);
        endSection(out);
        
        // Directions
        startSection(out, Section::Directions);
        const auto& directions = story.directions();
        writeSize(out, directions.size());
        for(const auto& dir: directions) writeString(out, dir);
        endSection(out);
        
        // Verbs
        startSection(out, Section::Verbs);
        const auto& verbs = story.verbs();
        writeSize(out, verbs.size());
        for(const auto& p: verbs) {
            writeEnum(out, Marker::VerbKind, p.second);
            writeString(out, p.first);
        }
        endSection(out);
        
        // Entities
        startSection(out, Section::Entities);
        writeSize(out, story.prototype.entities.size());
        
        for(const auto& e: story.prototype.entities) write(out, e.second);
        
        endSection(out);
    }
}