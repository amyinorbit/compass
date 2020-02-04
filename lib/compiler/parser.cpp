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
        lexer.nextToken();
        while(!match(Token::Kind::End)) {
            if(match("when")) {
                // TODO: match trigger
            } else {
                assertion();
            }
            // std::cout << "end of sentence: " << lexer.currentToken().type() << "\n";
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
        if(match(Grammar::Subjective) || match("there")) { return; }
        descriptor();
    }

    // descriptor     = ["the" / "a"] noun ["of" noun]
    void Parser::descriptor() {
        match_any({"a", "an", "the"});
        string first = words_until_any({"of", "is", "have", "has"});

        if(match("of")) {
            match_any({"a", "an", "the"});
            string second = words_until_any({"of", "is", "have", "has"});
            infer.select(second, first);
            std::cout << "infer.select(" << second << ", " << first << ")" << std::endl;
        } else {
            infer.select(first);
            std::cout << "infer.select(" << first << ")" << std::endl;
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

    // TODO: we need to stop words_until() at directions, so we can have sentences of the type:
    //          "Jane Doe is a person in the living room"
    void Parser::kind_or_property() {
        // While kinds are techincally vm-native objects, we are better off having a separate
        // method call and hard-coding them here, rather than have the inference engine do the
        // legwork of recognising words
        if(match("kind")) {
            expect("of");
            string prototype = words_until();
            infer.new_kind(prototype);
            std::cout << "infer.new_kind(" << prototype << ")" << std::endl;
        } else if(match("property")) {
            if(match("of")) {
                string what = words_until();
                infer.new_property(what);
            } else {
                infer.new_property();
            }
        } else if(match("value")) {
            expect("of");
            string property = words_until();
            infer.new_property_value(property);
        } else {
            string what = words_until(Grammar::Preposition);
            infer.set_kind(what);
            std::cout << "infer.is_kind(" << what << ")" << std::endl;
            if(have(Grammar::Preposition)) {
                string prep = eat();
                string container = noun_until({});
                infer.contained(prep, container);
                std::cout << "infer.contained(" << prep << ", " << container << ")" << std::endl;
            }
        }
    }


    void Parser::attributes() {
        do {
            if(have(Grammar::Preposition)) {
                string prep = eat();
                string container = noun_until({"and"});
                infer.contained(prep, container);
                std::cout << "infer.contained(" << prep << ", " << container << ")" << std::endl;
            } else {
                string attr = words_until_any({"of", "from", "and"});
                if(match_any({"of", "from"})) {
                    string loc = words_until("and");
                    infer.link_to(attr, loc);
                    std::cout << "infer.link_to(" << attr << "," << loc << ")" << std::endl;
                } else {
                    infer.set_property(attr);
                    std::cout << "infer.set_property(" << attr << ")" << std::endl;
                }
            }
        } while(match("and") || match(Token::Kind::Comma));
    }

    // has-sentence   = ("has" / "have") property-name
    void Parser::has_sentence() {
        do {
            match_any({"a", "an", "the"});
            string prop = words_until("and");
            // infer.
            std::cout << "infer.add_prop(" << prop << ")" << std::endl;
        } while(match("and") || match(Token::Kind::Comma));
    }

    // can-sentence   = "can" "be" participle
    void Parser::can_sentence() {
        do {
            match_any({"a", "an", "the"});
            string prop = words_until("and");
            std::cout << "infer.add_prop(" << prop << ")" << std::endl;
        } while(match("and") || match(Token::Kind::Comma));
    }

    string Parser::noun_until(const set<string>& stop) {
        match_any({"a", "an", "the"});
        return words_until_any(stop);
    }
}
