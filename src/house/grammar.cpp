//===--------------------------------------------------------------------------------------------===
// grammar.cpp - Implementation of the House Engine's BasicEnglsih grammar support class.
// This source is part of the House Engine
//
// Created on 2019-01-31 by Amy Parent <amy@amyparent.com>
// Copyright (c) 2019 Amy Parent
// Licensed under the MIT License
// =^•.•^=
//===--------------------------------------------------------------------------------------------===
#include <cassert>
#include <house/grammar.hpp>

namespace House {
    
     static std::string toLower(const std::string& str) {
         std::string lower = str;
         std::transform(lower.begin(), lower.end(), lower.begin(), ::tolower);
         return lower;
     }
     
     template <typename T>
     bool contains(const std::set<T>& set, const T& key) {
         return set.find(key) != set.end();
     }
    
    const std::set<std::string> BasicEnglish::subjectives_ = {
        "I", "you", "he", "she", "it", "we", "they"
    };
    
    const std::set<std::string> BasicEnglish::objectives_ = {
        "me", "you", "him", "her", "it", "us", "them"
    };
    
    const std::set<std::string> BasicEnglish::demonstratives_ = {
        "this", "that", "these", "those"
    };
    
    const std::set<std::string> BasicEnglish::prepositions_ = {
        "to", "off", "on", "under", "in"
    };
    
    const std::set<std::string> BasicEnglish::conjunctions_ = {
        "for", "and", "nor", "but", "or", "yet", "so"
    };
    
    // TODO: some words can be both (for example, 'it' is both an objective and subjective pronoun)
    // TODO: maybe return a tuple of values? or a std::set?
    // TODO: might also be better rewritng as is(string, class)->bool
    Grammar::Class BasicEnglish::classOf(const std::string& word) const {
        const auto low = toLower(word);
        if(low == "the") return Grammar::Definite;
        if(low == "a" || low == "an") return Grammar::Indefinite;
        if(contains(objectives_, low)) return Grammar::Objective;
        if(contains(subjectives_, low)) return Grammar::Subjective;
        if(contains(demonstratives_, low)) return Grammar::Demonstrative;
        if(contains(prepositions_, low)) return Grammar::Preposition;
        if(contains(conjunctions_, low)) return Grammar::Conjunction;
        return Grammar::Neutral;
    }
    
    bool BasicEnglish::meansBeing(const std::string& word) const {
        const auto low = toLower(word);
        return low == "is" || low == "are";
    }
    
    std::string BasicEnglish::objectiveOf(const std::string& pronoun) const {
        const auto low = toLower(pronoun);
        assert(contains(subjectives_, low));
        // TODO: implementation
        return "";
    }
    
    std::string BasicEnglish::subjectiveOf(const std::string& pronoun) const {
        const auto low = toLower(pronoun);
        assert(contains(subjectives_, low));
        // TODO: implementation
        return "";
    }
}
