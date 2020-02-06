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
        while(!match(Token::Kind::end)) {
            if(match("when")) {
                // TODO: match trigger
            } else {
                assertion();
            }
            // std::cout << "end of sentence: " << lexer.currentToken().type() << "\n";
            expect(Token::period, "sentences should end with a period");
        }
    }

    // ABNF = subject continuation
    void Parser::assertion() {
        subject();
        if(is_plural()) {
            if(match("are")) are_sentence();
            else error("unknown assertion verb");
        } else {
            if(match("is")) is_sentence();
            else error("unknown assertion verb");
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
        string first = words_until_any({"of", "is", "has", "and"});

        if(match("of")) {
            match_any({"a", "an", "the"});
            string second = words_until_any({"of", "is", "are", "have", "has", "and"});
            subjects_.push_back({second, first});
        } else {
            subjects_.push_back({first, nothing()});
            while(match(Token::comma) || match("and")) {
                match_any({"a", "an", "the"});
                string sub = words_until_any({"are", "have", "and"});
                subjects_.push_back({sub, nothing()});
            }
        }
    }

    // is-sentence    = ("is" / "are") (kind / adjectives / locator / prop-def)
    void Parser::is_sentence() {
        if(match(Grammar::Indefinite)) {
            if(match("kind")) {
                finish_new_kind();
            } else if(match("property")) {
                finish_new_property();
            } else {
                finish_set_kind();
            }
        } else {
            attributes();
        }
    }

    void Parser::are_sentence() {
        if(match("kinds"))
            finish_new_kind();
        else if(match("properties"))
            finish_new_property();
        else
            finish_set_kind();
    }

    void Parser::finish_new_property() {
        if(match("of")) {
            string what = words_until();
            each_subject([&]{ infer.new_property(what); });
        } else {
            each_subject([&]{ infer.new_property(); });
        }
    }

    void Parser::finish_new_kind() {
        expect("of");
        string prototype = words_until();
        each_subject([&]{ infer.new_kind(prototype); });
    }

    void Parser::finish_set_kind() {
        string what = words_until(Grammar::Preposition);
        if(is_plural()) what = infer.singular(what);
        each_subject([&]{ infer.is_a(what); });


        if(have(Grammar::Preposition)) {
            string prep = eat();
            string container = noun_until({});
            each_subject([&]{ infer.contained(prep, container); });
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
        } while(match("and") || match(Token::Kind::comma));
    }

    // has-sentence   = ("has" / "have") property-name
    void Parser::has_sentence() {
        do {
            match_any({"a", "an", "the"});
            string prop = words_until("and");
            std::cout << "infer.has_prop(" << prop << ")" << std::endl;
        } while(match("and") || match(Token::Kind::comma));
    }

    // can-sentence   = "can" "be" participle
    void Parser::can_sentence() {
        do {
            match_any({"a", "an", "the"});
            string prop = words_until("and");
        } while(match("and") || match(Token::Kind::comma));
    }

    string Parser::noun_until(const set<string>& stop) {
        match_any({"a", "an", "the"});
        return words_until_any(stop);
    }
}
