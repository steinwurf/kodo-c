// Copyright Steinwurf ApS 2014.
// Distributed under the "STEINWURF RESEARCH LICENSE 1.0".
// See accompanying file LICENSE.rst or
// http://www.steinwurf.com/licensing

#pragma once

#include "kodoc.h"

#include <cstdint>
#include <cassert>

#include <fifi/default_field.hpp>

#include <meta/typelist.hpp>

#include <kodo/enable_trace.hpp>
#include <kodo/api/api.hpp>


template<template<class, class, class...> class Coder, class... Interfaces>
kodo_factory_t
create_factory_wrapper(int32_t finite_field, uint32_t max_symbols,
                       uint32_t max_symbol_size, int32_t trace_mode)
{
    using namespace kodo;

    kodo_factory_t factory = 0;

    if (trace_mode == kodo_trace_disabled)
    {
        if (finite_field == kodo_binary)
        {
            auto f = std::make_shared<
                typename Coder<fifi::binary, meta::typelist<>, Interfaces...>::factory>(
                    max_symbols, max_symbol_size);

            factory = (kodo_factory*)f->keep_alive();
        }
        else if (finite_field == kodo_binary4)
        {
            auto f = std::make_shared<
                typename Coder<fifi::binary4, meta::typelist<>, Interfaces...>::factory>(
                    max_symbols, max_symbol_size);

            factory = (kodo_factory*)f->keep_alive();
        }
        else if (finite_field == kodo_binary8)
        {
            auto f = std::make_shared<
                typename Coder<fifi::binary8, meta::typelist<>, Interfaces...>::factory>(
                    max_symbols, max_symbol_size);

            factory = (kodo_factory*)f->keep_alive();
        }
        else if (finite_field == kodo_binary16)
        {
            auto f = std::make_shared<
                typename Coder<fifi::binary16, meta::typelist<>, Interfaces...>::factory>(
                    max_symbols, max_symbol_size);

            factory = (kodo_factory*)f->keep_alive();
        }
    }
    else
    {
        if (finite_field == kodo_binary)
        {
            auto f = std::make_shared<typename Coder<
                fifi::binary,
                meta::typelist<kodo::enable_trace>,
                kodo::api::trace_interface,
                Interfaces...>::factory>(max_symbols, max_symbol_size);

            factory = (kodo_factory*)f->keep_alive();
        }
        else if (finite_field == kodo_binary4)
        {
            auto f = std::make_shared<typename Coder<
                fifi::binary4,
                meta::typelist<kodo::enable_trace>,
                kodo::api::trace_interface,
                Interfaces...>::factory>(max_symbols, max_symbol_size);

            factory = (kodo_factory*)f->keep_alive();
        }
        else if (finite_field == kodo_binary8)
        {
            auto f = std::make_shared<typename Coder<
                fifi::binary8,
                meta::typelist<kodo::enable_trace>,
                kodo::api::trace_interface,
                Interfaces...>::factory>(max_symbols, max_symbol_size);

            factory = (kodo_factory*)f->keep_alive();
        }
        else if (finite_field == kodo_binary16)
        {
            auto f = std::make_shared<typename Coder<
                fifi::binary16,
                meta::typelist<kodo::enable_trace>,
                kodo::api::trace_interface,
                Interfaces...>::factory>(max_symbols, max_symbol_size);

            factory = (kodo_factory*)f->keep_alive();
        }
    }

    // Unknown field type
    assert(factory);

    return factory;
}
