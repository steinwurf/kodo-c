// Copyright Steinwurf ApS 2014.
// Distributed under the "STEINWURF RESEARCH LICENSE 1.0".
// See accompanying file LICENSE.rst or
// http://www.steinwurf.com/licensing

#pragma once

#include "kodoc.h"

#include <cstdint>
#include <cassert>
#include <map>
#include <string>

#include <kodo/api/api.hpp>
#include <kodo/runtime/runtime_encoder.hpp>
#include <kodo/runtime/runtime_decoder.hpp>

template<template<class,class,class...> class Coder, class... Interfaces>
kodo_factory_t
create_encoder_factory(int32_t finite_field, uint32_t max_symbols,
                       uint32_t max_symbol_size, int32_t trace_mode, bool deep_storage)
{
    using namespace kodo;

    kodo_factory_t factory = 0;

    std::map<int32_t, std::string> finite_field_map = {
        {kodo_binary, "binary"},
        {kodo_binary4, "binary4"},
        {kodo_binary8, "binary8"},
        {kodo_binary16, "binary16"},
        {kodo_prime2325, "prime2325"}
    };

    auto runtime_encoder = runtime::runtime_encoder<Coder, Interfaces...>();

    if (deep_storage)
    {
        runtime_encoder.set_deep_storage();
    }

    runtime_encoder.set_field(finite_field_map[finite_field]);

    if (trace_mode)
    {
        runtime_encoder.enable_trace();
    }

    auto f = runtime_encoder.build(max_symbols, max_symbol_size);

    factory = (kodo_factory*)f->keep_alive();

    // Unknown field type
    assert(factory);

    return factory;
}

template<template<class,class,class...> class Coder, class... Interfaces>
kodo_factory_t
create_decoder_factory(int32_t finite_field, uint32_t max_symbols,
                       uint32_t max_symbol_size, int32_t trace_mode, bool deep_storage)
{
    using namespace kodo;

    kodo_factory_t factory = 0;

    std::map<int32_t, std::string> finite_field_map = {
        {kodo_binary, "binary"},
        {kodo_binary4, "binary4"},
        {kodo_binary8, "binary8"},
        {kodo_binary16, "binary16"},
        {kodo_prime2325, "prime2325"}
    };

    auto runtime_decoder = runtime::runtime_decoder<Coder, Interfaces...>();

    if (deep_storage)
    {
        runtime_decoder.set_deep_storage();
    }

    runtime_decoder.set_field(finite_field_map[finite_field]);

    if (trace_mode)
    {
        runtime_decoder.enable_trace();
    }

    auto f = runtime_decoder.build(max_symbols, max_symbol_size);

    factory = (kodo_factory*)f->keep_alive();

    // Unknown field type
    assert(factory);

    return factory;
}
