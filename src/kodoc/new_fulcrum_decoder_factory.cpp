// Copyright Steinwurf ApS 2014.
// Distributed under the "STEINWURF RESEARCH LICENSE 1.0".
// See accompanying file LICENSE.rst or
// http://www.steinwurf.com/licensing

#include "kodoc.h"

#include <cstdint>

#include <kodo/api/api.hpp>
#include <kodo/rlnc/api/fulcrum_interface.hpp>
#include <kodo/rlnc/fulcrum_combined_decoder.hpp>

#include "create_factory.hpp"
#include "decoder_interface_aggregator.hpp"

namespace kodoc
{
    using fulcrum_decoder_interface = decoder_interface_aggregator<
        kodo::rlnc::api::fulcrum_interface>;

    kodo_factory_t new_fulcrum_decoder_factory(int32_t finite_field,
        uint32_t max_symbols, uint32_t max_symbol_size)
    {
        using namespace kodo;

        return create_factory<
            rlnc::fulcrum_combined_decoder,
            meta::typelist<storage_type<tag::mutable_shallow_storage>>,
            fulcrum_decoder_interface>(
                finite_field, max_symbols, max_symbol_size);
    }
}
