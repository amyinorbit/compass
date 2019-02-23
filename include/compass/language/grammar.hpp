//===--------------------------------------------------------------------------------------------===
// grammar.hpp - Basic grammar system for the Compass Engine.
// This source is part of the Compass Engine
//
// Created on 2019-01-31 by Amy Parent <amy@amyparent.com>
// Copyright (c) 2019 Amy Parent
// Licensed under the MIT License
// =^•.•^=
//===--------------------------------------------------------------------------------------------===
#pragma once
#include <string>
#include <set>

namespace Compass::Language {
    
    class Grammar {
    public:
        enum Class {
            Neutral, Preposition, Conjunction,
            Objective, Subjective, Possessive, Demonstrative, // Pronouns
            Definite, Indefinite                    // Articles
        };
        
        virtual ~Grammar() {}
        virtual Class classOf(const std::string& word) const = 0;
        virtual bool meansBeing(const std::string& word) const = 0;
        virtual bool is(const std::string& word, Class wordClass) const = 0;
    };
    
    
    class BasicEnglish : public Grammar {
    public:
        BasicEnglish() {}
        virtual ~BasicEnglish() {}

        virtual Grammar::Class classOf(const std::string& word) const;
        virtual bool meansBeing(const std::string& word) const;
        virtual bool is(const std::string& word, Class wordClass) const;
        
    private:
        static const std::set<std::string> subjectives_;
        static const std::set<std::string> objectives_;
        static const std::set<std::string> possessives_;
        static const std::set<std::string> demonstratives_;
        static const std::set<std::string> prepositions_;
        static const std::set<std::string> conjunctions_;
    };
}