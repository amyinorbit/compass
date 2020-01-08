//===--------------------------------------------------------------------------------------------===
// grammar.cpp - Implementation of the Compass Engine's BasicEnglsih grammar support class.
// This source is part of the Compass Engine
//
// Created on 2019-01-31 by Amy Parent <amy@amyparent.com>
// Copyright (c) 2019 Amy Parent
// Licensed under the MIT License
// =^•.•^=
//===--------------------------------------------------------------------------------------------===
#include <cassert>
#include <compass/language/grammar.hpp>

namespace amyinorbit::compass {

     template <typename T>
     bool contains(const std::set<T>& set, const T& key) {
         return set.find(key) != set.end();
     }

    const std::set<string> BasicEnglish::subjectives_ = {
        "I", "you", "he", "she", "it", "we", "they"
    };

    const std::set<string> BasicEnglish::objectives_ = {
        "me", "you", "him", "her", "it", "us", "them"
    };

    const std::set<string> BasicEnglish::possessives_ = {
        "my", "your", "his", "her", "its", "ours", "theirs"
    };

    const std::set<string> BasicEnglish::demonstratives_ = {
        "this", "that", "these", "those"
    };

    const std::set<string> BasicEnglish::prepositions_ = {
        "to", "off", "on", "under", "in", "at", "of", "from"
    };

    const std::set<string> BasicEnglish::conjunctions_ = {
        "for", "and", "nor", "but", "or", "yet", "so"
    };

    // TODO: some words can be both (for example, 'it' is both an objective and subjective pronoun)
    // TODO: maybe return a tuple of values? or a std::set?
    // TODO: might also be better rewritng as is(string, class)->bool
    Grammar::Class BasicEnglish::classOf(const string& word) const {
        const auto low = word;
        if(low == "the") return Grammar::Definite;
        if(low == "a" || low == "an") return Grammar::Indefinite;
        if(contains(objectives_, low)) return Grammar::Objective;
        if(contains(subjectives_, low)) return Grammar::Subjective;
        if(contains(possessives_, low)) return Grammar::Possessive;
        if(contains(demonstratives_, low)) return Grammar::Demonstrative;
        if(contains(prepositions_, low)) return Grammar::Preposition;
        if(contains(conjunctions_, low)) return Grammar::Conjunction;
        return Grammar::Neutral;
    }

    bool BasicEnglish::meansBeing(const string& word) const {
        const auto low = word;
        return low == "is" || low == "are";
    }

    bool BasicEnglish::is(const string& word, Grammar::Class wordClass) const {
        const auto low = word;
        switch(wordClass) {
            case Grammar::Definite: return low == "the";
            case Grammar::Indefinite: return low == "a" || low == "an";
            case Grammar::Objective: return contains(objectives_, low);
            case Grammar::Subjective: return contains(subjectives_, low);
            case Grammar::Possessive: return contains(possessives_, low);
            case Grammar::Demonstrative: return contains(demonstratives_, low);
            case Grammar::Preposition: return contains(prepositions_, low);
            case Grammar::Conjunction: return contains(conjunctions_, low);
            default: return true;
        }
        return false;
    }
}
