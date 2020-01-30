//===--------------------------------------------------------------------------------------------===
// assembler.hpp - Small assembly language parser for
//
// Created by Amy Parent <amy@amyparent.com>
// Copyright (c) 2019 Amy Parent
// Licensed under the MIT License
// =^•.•^=
//===--------------------------------------------------------------------------------------------===
#pragma once
#include <compass/types.hpp>

namespace amyinorbit::compass {

    class Assembler {
    public:
        Assembler(const string& source)
            : source_(source)
            , current_(source.unicode_scalars().begin())
            , start_(source.unicode_scalars().begin()) {}

    private:

        struct Token {

            enum Kind {
                string_literal,
                int_literal,
                float_literal,

                label,
                keyword,
                directive,
                instruction,

                colon,
                equals,
                new_line,

                brace_l,
                brace_r,
                bracket_l,
                bracket_r,
                angle_l,
                angle_r,

                end
            };

            Kind kind;
            string data;
        };

        void object();
        void objectField();
        void function();
        void instruction();

        bool is(Token::Kind kind) const { return token_.kind == kind; }
        void expect(Token::Kind kind, const string& message = "unexpected token");
        bool match(Token::Kind kind);
        void eat() { next_token(); }

        void error(const string& message) {
            std::cerr << "fatal error: " << message << "\n";
            abort();
        }

        // MARK: - Lexing Functions

        const Token& lex_string(char delim='"');
        const Token& lex_keyword(Token::Kind kind);

        unicode::scalar current() const { return *current_; }
        unicode::scalar next_char() { return *(current_++); }
        const Token& next_token();
        const Token& make_token(Token::Kind kind, const string& str = "") {
            token_ = {kind, str}; return token_;
        }

        string source_;
        unicode::scalar_iterator current_;
        unicode::scalar_iterator start_;
        Token token_;
    };
}
