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

    the "basic" structure of a sentence is therefore:
        [subject] [verb] [object|attributes] [qualifier]

    In ABNF:
        sentence    = subject verb (object / attribute) [qualifier] '.'
        subject     = 'there' / 'it' / 1*(non-verb-word)
        verb        = 'is' / 'lies'
        attributes  = kind / locator / (kind locator)
        kind        = ('a' / 'an') non-verb-word
        locator     = loc-relative noun
        noun        = ['the'] 1*(non-verb-word)
        qualifier   = ('which' / 'that') sentence
    */

    class AssertionParser : public RDParser {
    public:
        AssertionParser(const string& data, Driver& driver, InferEngine& infer)
            : RDParser(data, driver), infer(infer) {}
        virtual ~AssertionParser() {}

        void sentence();
    private:

        void subject();
        void verb();
        void kind();
        void qualifier();
        void adjectives();

        InferEngine& infer;
    };
}
