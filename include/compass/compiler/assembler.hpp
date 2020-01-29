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

    private:
        enum Kind {
            string_literal,
            int_literal,
            float_literal,

            keyword,
            directive,

            brace_l,
            brace_r,
            bracket_l,
            bracket_r,
            angle_l,
            angle_r,

            end
        };

        struct Token {
            Kind kind;
            string data;
        };
    };
}
