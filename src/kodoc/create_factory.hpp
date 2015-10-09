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
    template <class,class> class Coder,
    class Features,
    template<class> class CoderBinding,
    template<class> class FactoryBinding
>
kodo_factory_t create_factory(int32_t finite_field, uint32_t max_symbols,
                              uint32_t max_symbol_size)
{
    using namespace kodo;

    kodo_factory_t factory = 0;

    if (finite_field == kodo_binary)
    {
        using features = typename Features::template append<kodo::enable_trace>;
        using stack_type = Coder<fifi::binary, features>;
        using factory_type = typename stack_type::factory;
        using factory_binding = api::build_binding<
            CoderBinding<stack_type>, FactoryBinding<factory_type>>;

        auto binding = std::make_shared<factory_binding>();
        binding->set_stack(std::make_shared<factory_type>(
            max_symbols, max_symbol_size));

        factory = (kodo_factory_t)binding->keep_alive();
    }
    else if (finite_field == kodo_binary4)
    {
        using features = typename Features::template append<kodo::enable_trace>;
        using stack_type = Coder<fifi::binary4, features>;
        using factory_type = typename stack_type::factory;
        using factory_binding = api::build_binding<
            CoderBinding<stack_type>, FactoryBinding<factory_type>>;

        auto binding = std::make_shared<factory_binding>();
        binding->set_stack(std::make_shared<factory_type>(
            max_symbols, max_symbol_size));

        factory = (kodo_factory_t)binding->keep_alive();
    }
    else if (finite_field == kodo_binary8)
    {
        using features = typename Features::template append<kodo::enable_trace>;
        using stack_type = Coder<fifi::binary8, features>;
        using factory_type = typename stack_type::factory;
        using factory_binding = api::build_binding<
            CoderBinding<stack_type>, FactoryBinding<factory_type>>;

        auto binding = std::make_shared<factory_binding>();
        binding->set_stack(std::make_shared<factory_type>(
            max_symbols, max_symbol_size));

        factory = (kodo_factory_t)binding->keep_alive();
    }

    assert(factory && "Invalid finite field");

    return factory;
}
