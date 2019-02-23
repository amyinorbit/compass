//===--------------------------------------------------------------------------------------------===
// parser.hpp - Compass story functional-style parser
// This source is part of the Compass Engine
//
// Created on 2019-02-10 by Amy Parent <amy@amyparent.com>
// Copyright (c) 2019 Amy Parent
// Licensed under the MIT License
// =^•.•^=
//===--------------------------------------------------------------------------------------------===
#pragma once
#include <set>
#include <string>
#include <compass/compiler/sema.hpp>
#include <compass/core/functional.hpp>
#include <compass/language/grammar.hpp>
#include <compass/language/rdparser.hpp>

namespace Compass {
namespace Compiler {
    
    class Compiler;
    
    class Parser: public Language::RDParser {
    public:
        Parser(const std::string& source, Compiler& driver)
            : RDParser(source, driver) {}
        void run();
        
    private:
        enum SentenceKind { Decl, Spec };
        
        void error(const std::string& e);
        
        void recTitleAndAuthor();

        void recImport();
        
        // MARK: - there sentence parsing
        
        void recThereSentence();
        
        // MARK: - active sentence parsing
        
        void recActiveSentence();
        void recBeSentence(const optional<Noun>& subject);
        void recCanSentence(const optional<Noun>& subject);
        
        void recBeDecl(const optional<Noun>& subject);
        void recPropertyDecl(const optional<Noun>& subject);
        void recDirectionDecl(const std::string& dir);
        
        // MARK: - relative positioning parsing
        
        void recRelDirection();
        void recRelRelation();
        Relation::Kind recRelationLoc();
        
        // MARK: - parsing common blocks
        
        optional<Noun> recSubject();
        void recDescription();
        Entity::Kind recClass();
        Noun recNoun();
        
        bool haveDirection() const;
        Compiler& compiler() { return reinterpret_cast<Compiler&>(driver); }
        const Compiler& compiler() const { return reinterpret_cast<const Compiler&>(driver); }
    };
}
}
