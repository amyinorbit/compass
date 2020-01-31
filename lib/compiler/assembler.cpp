//===--------------------------------------------------------------------------------------------===
// assembler.cpp - Implementation of the Compass IR assembler
//
// Created by Amy Parent <amy@amyparent.com>
// Copyright (c) 2019 Amy Parent
// Licensed under the MIT License
// =^•.•^=
//===--------------------------------------------------------------------------------------------===
#include <compass/compiler/assembler.hpp>
#include <apfun/maybe.hpp>
#include "instr_data.inc"

namespace amyinorbit::compass {
    using namespace sema;

    void Assembler::compile() {
        next_token();

        while(!is(Token::end)) {
            if(match(Token::function_kw)) {
                function();
            }
            else if(match(Token::object_kw)) {
                object();
            }
            if(!match(Token::new_line)) break;
        }

        expect(Token::end, "expected end of file");
    }

    void Assembler::object() {
        auto name = text();
        maybe<string> prototype = nothing();
        expect(Token::string_literal);

        if(match(Token::angle_l)) {
            prototype = text();
            expect(Token::string_literal);
            expect(Token::angle_r);
        }

        expect(Token::brace_l);

        for(;;) {
            if(is(Token::string_literal)) object_field();
            if(!match(Token::new_line)) break;
        }

        expect(Token::brace_r);

    }

    void Assembler::object_field() {
        auto name = text();
        expect(Token::string_literal);
        expect(Token::equals);
        literal();
    }

    void Assembler::function() {
        auto name = text();
        expect(Token::string_literal);
        expect(Token::brace_l);

        for(;;) {
            if(is(Token::identifier)) eat(); // TODO: replace with label();
            if(is(Token::instruction)) instruction();
            if(!match(Token::new_line)) break;
        }

        expect(Token::brace_r);
    }

    void Assembler::instruction() {
        auto mnem = text();
        expect(Token::instruction, "invalid source (expected an instruction mnemonic)");
        std::cout << "instr: " << mnem;
        if(!is(Token::new_line) && !is(Token::end)) {
            operand(instruction_data.at(mnem).op);
        }
        std::cout << "\n";
    }

    void Assembler::label() {
        auto name = text();
        expect(Token::identifier, "labels must be a valid identifier");
        expect(Token::colon);
        // sema.label(name)
    }

    sema::Value Assembler::literal() {
        auto op = text();
        if(match(Token::int_literal)) {
            return Value((i32)std::atoi(op.data()));
        }
        else if(match(Token::float_literal)) {
            return Value((float)std::atof(op.data()));
        }
        else if(match(Token::string_literal)) {
            return Value(op);
        }
        else {
            error("invalid literal type");
        }
        return Value(nil_tag);
    }

    void Assembler::operand(Opcode inst) {
        literal();
        // else if(matc)
    }

    bool Assembler::match(Token::Kind kind) {
        if(is(kind)) {
            eat();
            return true;
        }
        return false;
    }

    void Assembler::expect(Token::Kind kind, const string& message) {
        if(!match(kind)) {
            if(is(Token::invalid))
                error(token_.data);
            else
                error(message);
        }
    }

    static inline bool is_digit(unicode::scalar s) {
        return s >= '0' && s <= '9';
    }

    const Assembler::Token& Assembler::lex_number() {
        bool is_minus = false;
        if(current() == '-') {
            is_minus = true;
            next_char();
        }
        while(is_digit(current())) next_char();

        if(current() != '.')
            return make_token(Token::int_literal, string(start_.utf8(), current_.utf8()));

        if(!is_digit(current())) return make_token(Token::invalid);

        while(is_digit(current())) next_char();
        return make_token(Token::float_literal, string(start_.utf8(), current_.utf8()));
    }

    const Assembler::Token& Assembler::lex_string(char delim) {
        start_ = current_;
        next_char();
        while(current() != delim) {
            auto c = next_char();
            if(c == '\0') return make_token(Token::invalid, "quoted string not finished");
        }
        auto end = current_;
        next_char();
        return make_token(Token::string_literal, string(start_.utf8(), end.utf8()));
    }

    const Assembler::Token& Assembler::lex_keyword() {
        while(current().is_identifier()) {
            next_char();
        }
        string str(start_.utf8(), current_.utf8());
        if(str == "%obj") return make_token(Token::object_kw);
        if(str == "%fn") return make_token(Token::function_kw);
        return make_token(Token::invalid, "unknown keyword '" + str + "'");
    }

    const Assembler::Token& Assembler::lex_ident() {
        while(current().is_identifier()) {
            next_char();
        }
        string str(start_.utf8(), current_.utf8());
        bool is_instruction = instruction_data.count(str) != 0;
        return make_token(is_instruction ? Token::instruction : Token::identifier, str);
    }

    void Assembler::lex_comment() {
        while(current().is_valid() && current().value != '\n') {
            next_char();
        }
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
                case 0xfeff:
                    break;

                case '\n':
                    return make_token(Token::new_line);

                case ':':
                    return make_token(Token::colon);
                case '=':
                    return make_token(Token::equals);

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
                    return lex_keyword();

                case '"':
                case '\'':
                    return lex_string(c.value);

                case '!':
                case ';':
                    lex_comment();
                    break;

                default:
                    if(c.is_identifier_head()) {
                        return lex_ident();
                    }
                    std::cout << "invalid: " << c << "\n";
                    return make_token(Token::invalid, "invalid character in source");
            }
        }
        return make_token(Token::end);
    }
}
