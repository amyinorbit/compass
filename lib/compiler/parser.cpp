//===--------------------------------------------------------------------------------------------===
// parser.cpp - Assertion parser Implementation
//
// Created by Amy Parent <amy@amyparent.com>
// Copyright (c) 2019 Amy Parent
// Licensed under the MIT License
// =^•.•^=
//===--------------------------------------------------------------------------------------------===
#include <compass/compiler/parser.hpp>
#include <apfun/view.hpp>

namespace amyinorbit::compass {

    using namespace fp;
    void Parser::parse() {
        while(!match(Token::Kind::End)) {
            if(match("when")) {
                // TODO: match trigger
            } else {
                assertion();
            }
            expect(Token::Period, "sentences should end with a period");
        }
    }

    // ABNF = subject continuation
    void Parser::assertion() {
        subject();
        if(match_being()) {
            is_sentence();
        } else if(match("can")) {
            can_sentence();
        } else if(match("has")) {
            has_sentence();
        } else {
            // TODO: throw error()
        }
    }

    // subject        = "there" / "it" / descriptor
    void Parser::subject() {
        if(match_any({"there", "it"})) { return; }
        descriptor();
    }

    // descriptor     = ["the" / "a"] noun ["of" noun]
    void Parser::descriptor() {
        match(Grammar::Indefinite) || match(Grammar::Definite);
        string first = words_until_any({"of", "is", "have", "has"});
        if(match("of")) {
            // infer.select(first)
        } else {
            match(Grammar::Indefinite) || match(Grammar::Definite);
            string second = words_until_any({"of", "is", "have", "has"});
            // infer.select(second, first);
        }
    }

    // is-sentence    = ("is" / "are") (kind / adjectives / locator / prop-def)
    void Parser::is_sentence() {
        if(match(Grammar::Indefinite)) {
            kind_or_property();
        } else {
            attributes();
        }
    }

    void Parser::kind_or_property() {
        // While kinds are techincally vm-native objects, we are better off having a separate
        // method call and hard-coding them here, rather than have the inference engine do the
        // legwork of recognising words
        if(match("kind")) {
            expect("of");
            string prototype = words();
            // infer.new_kind(prototype);
        } else if(match("property")) {
            expect("of");
            string what = words();
            // infer.new_property(what);
        } else {
            string what = words();
            // infer.is_kind(what)
        }
    }


    void Parser::attributes() {
        do {
            if(have(Grammar::Preposition)) {
                string prep = eat();
                string container = words("and");
                // infer.contained(prep, container);
            } else {
                string attr = words_until_any({"of", "from", "and"});
                if(match_any({"of", "from"})) {
                    string loc = words("and");
                    // infer.link_to(loc, attr);
                } else {
                    /// infer.set_property(attr);
                }
            }
            // infer.property(adj);
        } while(match("and") || match(Token::Kind::Comma));
    }

    // has-sentence   = ("has" / "have") property-name
    void Parser::has_sentence() {
        do {
            match(Grammar::Indefinite) || match(Grammar::Definite);
            string prop = words("and");
            // infer.add_prop(prop);
        } while(match("and") || match(Token::Kind::Comma));
    }

    // can-sentence   = "can" "be" participle
    void Parser::can_sentence() {
        do {
            match(Grammar::Indefinite) || match(Grammar::Definite);
            string participle = words("and");
            // infer.add_prop(prop);
        } while(match("and") || match(Token::Kind::Comma));
    }
}
