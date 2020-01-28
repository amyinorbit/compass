//===--------------------------------------------------------------------------------------------===
// unpack.hpp - Story file deserialiser
//
// Created by Amy Parent <amy@amyparent.com>
// Copyright (c) 2019 Amy Parent
// Licensed under the MIT License
// =^•.•^=
//===--------------------------------------------------------------------------------------------===
#pragma once
#include <compass/types.hpp>
#include <compass/runtime2/type.hpp>
#include <compass/runtime2/collector.hpp>
#include <compass/runtime2/bin_io.hpp>
#include <iostream>

namespace amyinorbit::compass {

    class Loader {
    public:
        Loader(rt::Collector& collector, std::istream& in)
            : collector_(collector), reader_(in) {}

        void load();

    private:

        bool signature();
        void object();
        void constant();
        rt::Value value();

        void utf8();
        void list();

        vector<rt::Object*> objects_;
        vector<rt::Value> constants_;

        rt::Collector& collector_;
        BinaryReader reader_;
    };
}
