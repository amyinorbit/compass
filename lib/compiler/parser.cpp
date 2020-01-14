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

    // #define DEBUG() (std::cout << __PRETTY_FUNCTION__ << "\n")
#define DEBUG()

    using namespace fp;
    void Parser::parse() { DEBUG();
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
    void Parser::assertion() { DEBUG();
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
    void Parser::subject() { DEBUG();
        if(match_any({"there", "it"})) { return; }
        descriptor();
    }

    // descriptor     = ["the" / "a"] noun ["of" noun]
    void Parser::descriptor() { DEBUG();
        match_any({"a", "an", "the"});
        string first = words_until_any({"of", "is", "have", "has"});

        if(match("of")) {
            match_any({"a", "an", "the"});
            string second = words_until_any({"of", "is", "have", "has"});
            std::cout << "infer.select(" << second << ", " << first << ")" << std::endl;
        } else {
            std::cout << "infer.select(" << first << ")" << std::endl;
        }
    }

    // is-sentence    = ("is" / "are") (kind / adjectives / locator / prop-def)
    void Parser::is_sentence() { DEBUG();
        if(match(Grammar::Indefinite)) {
            kind_or_property();
        } else {
            attributes();
        }
    }

    // TODO: we need to stop words() at directions, so we can have sentences of the type:
    //          "Jane Doe is a person in the living room"
    void Parser::kind_or_property() { DEBUG();
        // While kinds are techincally vm-native objects, we are better off having a separate
        // method call and hard-coding them here, rather than have the inference engine do the
        // legwork of recognising words
        if(match("kind")) {
            expect("of");
            string prototype = words();
            std::cout << "infer.new_kind(" << prototype << ")" << std::endl;
        } else if(match("property")) {
            expect("of");
            string what = words();
            std::cout << "infer.new_property(" << what << ")" << std::endl;
        } else {
            string what = words();
            std::cout << "infer.is_kind(" << what << ")" << std::endl;
        }
    }


    void Parser::attributes() { DEBUG();
        do {
            if(have(Grammar::Preposition)) {
                string prep = eat();
                string container = noun_until({"and"});
                std::cout << "infer.contained(" << prep << ", " << container << ")" << std::endl;
            } else {
                string attr = words_until_any({"of", "from", "and"});
                if(match_any({"of", "from"})) {
                    string loc = words("and");
                    std::cout << "infer.link_to(" << loc << ")" << std::endl;
                } else {
                    std::cout << "infer.set_property(" << attr << ")" << std::endl;
                }
            }
        } while(match("and") || match(Token::Kind::Comma));
    }

    // has-sentence   = ("has" / "have") property-name
    void Parser::has_sentence() { DEBUG();
        do {
            match_any({"a", "an", "the"});
            string prop = words("and");
            std::cout << "infer.add_prop(" << prop << ")" << std::endl;
        } while(match("and") || match(Token::Kind::Comma));
    }

    // can-sentence   = "can" "be" participle
    void Parser::can_sentence() { DEBUG();
        do {
            match_any({"a", "an", "the"});
            string prop = words("and");
            std::cout << "infer.add_prop(" << prop << ")" << std::endl;
        } while(match("and") || match(Token::Kind::Comma));
    }

    string Parser::noun_until(const set<string>& stop) {
        match_any({"a", "an", "the"});
        return words_until_any(stop);
    }
}
