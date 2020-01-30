//===--------------------------------------------------------------------------------------------===
// assembler.cpp - Implementation of the Compass IR assembler
//
// Created by Amy Parent <amy@amyparent.com>
// Copyright (c) 2019 Amy Parent
// Licensed under the MIT License
// =^•.•^=
//===--------------------------------------------------------------------------------------------===
#include <compass/compiler/assembler.hpp>

namespace amyinorbit::compass {

    bool Assembler::match(Token::Kind kind) {
        if(is(kind)) {
            eat();
            return true;
        }
        return false;
    }

    void Assembler::expect(Token::Kind kind, const string& message) {
        if(!match(kind)) error(message);
    }

    const Assembler::Token& Assembler::lex_string(char delim) {
        start_ = current_;
        while(current() != delim) {
            auto c = next_char();
            if(c == '\0') {
                std::cerr << "quoted string not finished\n";
                abort();
            }
        }
        auto end = current_;
        next_char();
        return make_token(Token::string_literal, string(start_.utf8(), end.utf8()));
    }

    const Assembler::Token& Assembler::lex_keyword(Token::Kind kind) {
        while(current().is_identifier()) {
            next_char();
        }
        return make_token(kind, string(start_.utf8(), current_.utf8()));
    }

    const Assembler::Token& Assembler::next_token() {
        if(current_ == source_.unicode_scalars().end()) return make_token(Token::end);

        while(current().is_valid()) {
            start_ = current_;
            auto c = next_char();

            switch(c.value) {
                //whitespace
                case 0x0020:
                case 0x000d:
                case 0x0009:
                case 0x000b:
                case 0x000c:
                case 0x000a:
                case 0xfeff:
                    break;

                case '<':
                    return make_token(Token::angle_l);
                case '>':
                    return make_token(Token::angle_r);
                case '{':
                    return make_token(Token::brace_l);
                case '}':
                    return make_token(Token::brace_r);
                case '[':
                    return make_token(Token::bracket_l);
                case ']':
                    return make_token(Token::bracket_r);


                case '%':
                    return lex_keyword(Token::directive);

                case '.':
                    return lex_keyword(Token::label);

                case '"':
                case '\'':
                    return lex_string(c.value);

                case '!':
                    // skip_comment();
                    break;

                default:
                    if(c.is_identifier_head()) {
                        return lex_keyword(Token::keyword);
                    }
                    std::cerr << "Invalid character: "<< c <<"\n";
                    abort();
            }
        }
        return make_token(Token::end);
    }
}
