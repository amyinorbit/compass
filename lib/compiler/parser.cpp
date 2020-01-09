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
    /*
    sentence    = subject verb (object / attribute) [qualifier] '.'
    subject     = 'there' / 'it' / 1*(non-verb-word)
    verb        = 'is' / 'lies'
    attributes  = kind / locator / (kind locator)
    kind        = ('a' / 'an') non-verb-word
    locator     = loc-relative noun
    noun        = ['the'] 1*(non-verb-word)
    qualifier   = ('which' / 'that') sentence
    */

    auto printer = [](const auto& value) { std::cout << "  - " << value << "\n"; };

    void AssertionParser::sentence() {
        std::cout << "subject: " << subject() << "\n";
        std::cout << "verb: " << verb() << "\n";

        if(match(Grammar::Class::Indefinite)) {
            std::cout << "kind: " << kind() << "\n";
            if(match("which") || match("where")) {
                qualifier();
            }
        } else {
            std::cout << "adjectives:\n";
            adjectives() | view::tapped(printer);
        }
        // TODO: qualifier
    }

    string AssertionParser::subject() {
        match(Grammar::Class::Indefinite);
        string subject;

        do {
            if(subject.size()) subject += " ";
            subject += text();
            expect(Token::Kind::Word, "This sentence is missing a subject");
        } while(have(Token::Kind::Word) && !haveBeing());

        return subject;
    }

    string AssertionParser::verb() {
        string v = text();
        expectBeing("This assertion is missing a valid verb");
        return v;
    }

    string AssertionParser::kind() {
        string kind;
        do {
            if(kind.size()) kind += " ";
            kind += text();
            expect(Token::Kind::Word, "This sentence is missing a kind");
        } while(have(Token::Kind::Word) && !have(Grammar::Class::Preposition));

        return kind;
    }

    std::vector<string> AssertionParser::adjectives() {
        std::vector<string> adjs;

        do {
            string adj = text();
            expect(Token::Kind::Word, "missing adjectives qualifying the thing");
            adjs.push_back(adj);
        } while(match(Token::Kind::Comma) || match("and"));

        return adjs;
    }
    
    void AssertionParser::qualifier() {
        std::cout << "---qualifier\n";
        std::cout << "   verb: " << verb() << "\n";
        
    }
}
