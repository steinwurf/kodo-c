// Copyright Steinwurf ApS 2014.
// Distributed under the "STEINWURF RESEARCH LICENSE 1.0".
// See accompanying file LICENSE.rst or
// http://www.steinwurf.com/licensing

#pragma once

#include "kodoc.h"

#include <cstdint>
#include <cassert>

#include <kodo/api/api.hpp>

template
<
    template <class,class,class...> class Coder,
    class Features,
    template<class> class FactoryBinding,
    template<class> class CoderBinding
>
kodo_factory_t create_factory(int32_t finite_field, uint32_t max_symbols,
    uint32_t max_symbol_size)
{
    using namespace kodo;

    kodo_factory_t factory = 0;

    if (finite_field == kodo_binary)
    {
        using result = typename Features::template append<kodo::enable_trace>;
        using stack_type = Coder<fifi::binary, result>;
        using factory_type = typename stack_type::factory;
        using binding_type =
            api::build_binding<CoderBinding<stack_type>, FactoryBinding<factory_type>>;

        auto binding = std::make_shared<binding_type>();
        binding->set_stack(std::make_shared<factory_type>(
            max_symbols, max_symbol_size));

        factory = (kodo_factory_t)binding->keep_alive();
    }
    else if (finite_field == kodo_binary4)
    {
        using result = typename Features::template append<kodo::enable_trace>;
        using stack_type = Coder<fifi::binary4, result>;
        using factory_type = typename stack_type::factory;
        using binding_type =
            api::build_binding<CoderBinding<stack_type>, FactoryBinding<factory_type>>;

        auto binding = std::make_shared<binding_type>();
        binding->set_stack(std::make_shared<factory_type>(
            max_symbols, max_symbol_size));

        factory = (kodo_factory_t)binding->keep_alive();
    }
    else if (finite_field == kodo_binary8)
    {
        using result = typename Features::template append<kodo::enable_trace>;
        using stack_type = Coder<fifi::binary8, result>;
        using factory_type = typename stack_type::factory;
        using binding_type =
            api::build_binding<CoderBinding<stack_type>, FactoryBinding<factory_type>>;

        auto binding = std::make_shared<binding_type>();
        binding->set_stack(std::make_shared<factory_type>(
            max_symbols, max_symbol_size));

        factory = (kodo_factory_t)binding->keep_alive();
    }

    assert(factory);

    return factory;
}