// Copyright Steinwurf ApS 2014.
// Distributed under the "STEINWURF RESEARCH LICENSE 1.0".
// See accompanying file LICENSE.rst or
// http://www.steinwurf.com/licensing

#include "kodoc.h"

#include <cstdint>

#include <kodo/api/api.hpp>
#include <kodo/rlnc/on_the_fly_decoder.hpp>

#include "create_factory.hpp"
#include "kodoc_runtime_decoder.hpp"

namespace kodoc
{
    template<class Stack>
    using on_the_fly_decoder_binding =
        kodo::api::partial_decoding_binding<
        kodo::api::write_payload_binding<Stack>>;

    kodo_factory_t new_on_the_fly_decoder_factory(int32_t finite_field,
        uint32_t max_symbols, uint32_t max_symbol_size)
    {
        using namespace kodo;
        return create_factory<
            kodoc_runtime_decoder<
            rlnc::on_the_fly_decoder,
            on_the_fly_decoder_binding>>(
                finite_field, max_symbols, max_symbol_size);
    }
}
