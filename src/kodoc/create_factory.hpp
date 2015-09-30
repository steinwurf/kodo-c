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
    class Features,
    class Interface
>
kodo_factory_t create_factory(int32_t finite_field, uint32_t max_symbols,
    uint32_t max_symbol_size)
{
    using namespace kodo;

    kodo_factory_t factory = 0;

    if (finite_field == kodo_binary)
    {
        using stack_factory = kodo::runtime::wrap_build<typename Coder<
            fifi::binary,
            typename Features::template append<kodo::enable_trace>,
            Interface>::factory>;

        auto f = std::make_shared<stack_factory>(
            max_symbols, max_symbol_size);

        factory = (kodo_factory_t)f->keep_alive();

    }
    else if (finite_field == kodo_binary4)
    {
        using stack_factory = kodo::runtime::wrap_build<typename Coder<
            fifi::binary4,
            typename Features::template append<kodo::enable_trace>,
            Interface>::factory>;

        auto f = std::make_shared<stack_factory>(
            max_symbols, max_symbol_size);

        factory = (kodo_factory_t)f->keep_alive();
    }
    else if (finite_field == kodo_binary8)
    {
        using stack_factory = kodo::runtime::wrap_build<typename Coder<
            fifi::binary8,
            typename Features::template append<kodo::enable_trace>,
            Interface>::factory>;

        auto f = std::make_shared<stack_factory>(
            max_symbols, max_symbol_size);

        factory = (kodo_factory_t)f->keep_alive();
    }

    assert(factory);

    return factory;
}
