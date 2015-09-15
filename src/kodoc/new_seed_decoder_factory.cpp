// Copyright Steinwurf ApS 2014.
// Distributed under the "STEINWURF RESEARCH LICENSE 1.0".
// See accompanying file LICENSE.rst or
// http://www.steinwurf.com/licensing

#include "kodoc.h"

#include <cstdint>

#include <kodo/api/api.hpp>
#include <kodo/rlnc/seed_decoder.hpp>

#include "create_factory.hpp"
#include "decoder_interface_aggregator.hpp"

namespace kodoc
{
    using seed_decoder_interface = decoder_interface_aggregator<>;

    kodo_factory_t new_seed_decoder_factory(int32_t finite_field,
        uint32_t max_symbols, uint32_t max_symbol_size)
    {
        using namespace kodo;

        return create_factory<
            rlnc::seed_decoder,
            meta::typelist<storage_type<tag::mutable_shallow_storage>>,
            seed_decoder_interface>(
                finite_field, max_symbols, max_symbol_size);
    }
}
