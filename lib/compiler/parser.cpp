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
        lexer.next_token();
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
        subjects_.clear();
        if(match(Grammar::Subjective)) { return; }
        descriptor();
    }

    // descriptor     = ["the" / "a"] noun ["of" noun]
    void Parser::descriptor() {
        match_any({"a", "an", "the"});
        string first = words_until_any({"of", "is", "have", "has"});

        if(match("of")) {
            match_any({"a", "an", "the"});
            string second = words_until_any({"of", "is", "have", "has"});
            subjects_.push_back({second, first});
        } else {
            subjects_.push_back({first, nothing()});
            while(match(Token::Comma) || match("and")) {
                match_any({"a", "an", "the"});
                string sub = words_until_any({"is", "have", "has", "and"});
                subjects_.push_back({sub, nothing()});
            }
        }
    }

    // is-sentence    = ("is" / "are") (kind / adjectives / locator / prop-def)
    void Parser::is_sentence() {
        if(match(Grammar::Indefinite) || have("values") || have("kinds")) {
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
        if(match("kind") || match("kinds")) {
            expect("of");
            string prototype = words_until();
            each_subject([&]{ infer.new_kind(prototype); });
        } else if(match("property")) {
            if(match("of")) {
                string what = words_until();
                each_subject([&]{ infer.new_property(what); });
            } else {
                each_subject([&]{ infer.new_property(); });
            }
        } else if(match("value") || match("values")) {
            expect("of");
            string property = words_until();
            each_subject([&]{ infer.new_property_value(property); });
        } else {
            string what = words_until(Grammar::Preposition);
            each_subject([&]{ infer.set_kind(what); });
            if(have(Grammar::Preposition)) {
                string prep = eat();
                string container = noun_until({});
                each_subject([&]{ infer.contained(prep, container); });
            }
        }
    }


    void Parser::attributes() {
        do {
            if(have(Grammar::Preposition)) {
                string prep = eat();
                string container = noun_until({"and"});
                each_subject([&]{ infer.contained(prep, container); });
            } else {
                string attr = words_until_any({"of", "from", "and"});
                if(match_any({"of", "from"})) {
                    string loc = words_until("and");
                    each_subject([&]{ infer.link_to(attr, loc); });
                } else {
                    each_subject([&]{ infer.set_property(attr); });
                }
            }
        } while(match("and") || match(Token::Kind::Comma));
    }

    // has-sentence   = ("has" / "have") property-name
    void Parser::has_sentence() {
        do {
            match_any({"a", "an", "the"});
            string prop = words_until("and");
            std::cout << "infer.has_prop(" << prop << ")" << std::endl;
        } while(match("and") || match(Token::Kind::Comma));
    }

    // can-sentence   = "can" "be" participle
    void Parser::can_sentence() {
        do {
            match_any({"a", "an", "the"});
            string prop = words_until("and");
        } while(match("and") || match(Token::Kind::Comma));
    }

    string Parser::noun_until(const set<string>& stop) {
        match_any({"a", "an", "the"});
        return words_until_any(stop);
    }
}
