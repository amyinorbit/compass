//===--------------------------------------------------------------------------------------------===
// parser.hpp - Parser for the compass assertion/inference compiler system
//
// Created by Amy Parent <amy@amyparent.com>
// Copyright (c) 2019 Amy Parent
// Licensed under the MIT License
// =^•.•^=
//===--------------------------------------------------------------------------------------------===
#pragma once
#include <compass/language/rdparser.hpp>
#include <compass/compiler/infer.hpp>
#include <vector>

namespace amyinorbit::compass {

    /*
    The assertion parser is a sort-of recursive descent parser.
    The goal is to parse sentences that assert something, AKA the compass authoring language.

    For simplicity's sake, we narrow ourselves to a handful of sentence types:

     * the kettle is a thing in the living room.
     * the living room is a place north of the patio.
     * the cup is on the table
     * the cupboard is a container which is big, blue, and old.
     * it is red and small.
     * there is a pot, a kettle and a cup on the table.

     * north is a direction. the opposite of north is south.
     * north is a direction which is the opposite of south.

     ABNF grammar

         sentence       = trigger / assertion

         assertion      = subject continuation
         continuation   = is-sentence / has-sentence / can-sentence

         is-sentence    = ('is' / 'are') (kind / adjectives / locator / prop-def)
         kind           = ('a' / 'an') ['kind' 'of']
         adjectives     = word *(',' word)
         locator        =  direction ('from' / 'of') noun
         prop-def       = 'a' 'property' 'of' noun

         has-sentence   = ('has' / 'have') property-name

         can-sentence   = 'can' 'be' participle

         subject        = 'there' / 'it' / descriptor
         descriptor     = ['the' / 'a'] noun ['of' noun]
    */

    class Parser : public RDParser {
    public:
        using Subject = vector<InferEngine::Ref>;

        Parser(const string& data, Driver& driver, InferEngine& infer)
            : RDParser(data, driver), infer(infer) {}
        virtual ~Parser() {}

        void parse();

    private:

        template <typename F>
        void each_subject(F&& f) {
            if(!subjects_.size()) {
                std::invoke(std::forward<F>(f));
            } else {
                for(const auto& s: subjects_) {
                    infer.select(s);
                    std::invoke(std::forward<F>(f));
                }
            }
        }

        bool is_plural() const { return subjects_.size() > 1; }
        void assertion();

        void subject();
        void noun();
        void descriptor();

        void is_sentence();
        void are_sentence();
        void attributes();
        void containment();

        void finish_set_kind();
        void finish_new_property();
        void finish_new_kind();

        void has_sentence();
        void can_sentence();

        string noun_until(const set<string>& stop);

        Subject subjects_;
        InferEngine& infer;
    };
}
