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
#include <kodo/runtime/wrap_build.hpp>


template
<
    template <class,class,class...> class Coder,
    class... Interfaces
>
kodo_factory_t create_factory(int32_t finite_field, uint32_t max_symbols,
    uint32_t max_symbol_size, int32_t trace_mode, int32_t storage_mode)
{
    using namespace kodo;

    kodo_factory_t factory = 0;

    if (trace_mode == kodo_trace_enabled)
    {
        if (finite_field == kodo_binary)
        {
            if (storage_mode == kodo_deep_storage)
            {
                using stack_factory = kodo::runtime::wrap_build<typename Coder<
                    fifi::binary,
                    meta::typelist<>,
                    Interfaces...>::factory>;


                auto f = std::make_shared<stack_factory>(
                    max_symbols, max_symbol_size);

                factory = (kodo_factory_t)f->keep_alive();
            }
            else if (storage_mode == kodo_shallow_storage)
            {

            }
        }
        else if (finite_field == kodo_binary4)
        {
            if (storage_mode == kodo_deep_storage)
            {

            }
            else if (storage_mode == kodo_shallow_storage)
            {

            }
        }
        else if (finite_field == kodo_binary8)
        {
            if (storage_mode == kodo_deep_storage)
            {

            }
            else if (storage_mode == kodo_shallow_storage)
            {

            }
        }
    }
    else if (trace_mode == kodo_trace_disabled)
    {

    }
    // std::map<int32_t, std::string> finite_field_map =
    // {
    //     {kodo_binary, "binary"},
    //     {kodo_binary4, "binary4"},
    //     {kodo_binary8, "binary8"}
    // };

    // auto r = Runtime();

    // if (storage_mode == kodo_shallow_storage)
    // {
    //     r.set_shallow_storage();
    // }

    // r.set_field(finite_field_map[finite_field]);

    // if (trace_mode)
    // {
    //     r.enable_trace();
    // }

    // auto f = r.build(max_symbols, max_symbol_size);

    // kodo_factory_t factory = (kodo_factory_t)f->keep_alive();

    assert(factory);

    return factory;
}
