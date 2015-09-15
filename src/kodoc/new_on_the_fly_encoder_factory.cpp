// Copyright Steinwurf ApS 2014.
// Distributed under the "STEINWURF RESEARCH LICENSE 1.0".
// See accompanying file LICENSE.rst or
// http://www.steinwurf.com/licensing

#include "kodoc.h"

#include <cstdint>

#include <kodo/api/api.hpp>
#include <kodo/rlnc/on_the_fly_encoder.hpp>

#include "create_factory.hpp"
#include "encoder_interface_aggregator.hpp"

namespace kodoc
{
    using on_the_fly_encoder_interface = encoder_interface_aggregator<
        kodo::api::systematic_interface>;

    kodo_factory_t new_on_the_fly_encoder_factory(int32_t finite_field,
        uint32_t max_symbols, uint32_t max_symbol_size)
    {
        using namespace kodo;

        return create_factory<
            rlnc::on_the_fly_encoder,
            meta::typelist<storage_type<tag::const_shallow_storage>>,
            on_the_fly_encoder_interface>(
                finite_field, max_symbols, max_symbol_size);
    }
}
