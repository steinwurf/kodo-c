// Copyright Steinwurf ApS 2014.
// Distributed under the "STEINWURF RESEARCH LICENSE 1.0".
// See accompanying file LICENSE.rst or
// http://www.steinwurf.com/licensing

#if !defined(KODOC_DISABLE_RLNC) && !defined(KODOC_DISABLE_ON_THE_FLY)

#include "kodoc.h"

#include <cstdint>

#include <kodo_core/api/api.hpp>
#include <kodo_rlnc/on_the_fly_decoder.hpp>
#include <kodo_rlnc/on_the_fly_encoder.hpp>

#include "create_factory.hpp"
#include "runtime_decoder.hpp"
#include "runtime_encoder.hpp"

namespace kodoc
{
kodoc_factory_t new_on_the_fly_encoder_factory(
    int32_t finite_field, uint32_t max_symbols, uint32_t max_symbol_size)
{
    return create_factory<
           runtime_encoder<
           kodo_rlnc::on_the_fly_encoder,
           kodo_core::api::systematic_binding>>(
               finite_field, max_symbols, max_symbol_size);
}

template<class Stack>
using on_the_fly_decoder_binding =
    kodo_core::api::partial_decoding_binding<
    kodo_core::api::symbol_decoding_status_updater_binding<
    kodo_core::api::write_payload_binding<Stack>>>;

kodoc_factory_t new_on_the_fly_decoder_factory(
    int32_t finite_field, uint32_t max_symbols, uint32_t max_symbol_size)
{
    return create_factory<
           runtime_decoder<
           kodo_rlnc::on_the_fly_decoder,
           on_the_fly_decoder_binding>>(
               finite_field, max_symbols, max_symbol_size);
}
}

#endif
