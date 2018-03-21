// Copyright Steinwurf ApS 2014.
// Distributed under the "STEINWURF EVALUATION LICENSE 1.0".
// See accompanying file LICENSE.rst or
// http://www.steinwurf.com/licensing

#if !defined(KODOC_DISABLE_RLNC) && !defined(KODOC_DISABLE_PERPETUAL)

#include "kodoc.h"

#include <cstdint>

#include <kodo_core/api/api.hpp>
#include <kodo_rlnc/perpetual_encoder.hpp>
#include <kodo_rlnc/perpetual_decoder.hpp>
#include <kodo_rlnc/api/perpetual_encoder_binding.hpp>

#include "create_factory.hpp"
#include "runtime_encoder.hpp"
#include "runtime_decoder.hpp"

namespace kodoc
{
kodoc_factory_t new_perpetual_encoder_factory(
    int32_t finite_field, uint32_t max_symbols, uint32_t max_symbol_size)
{
    return create_factory<
           runtime_encoder<
           kodo_rlnc::perpetual_encoder,
           kodo_rlnc::api::perpetual_encoder_binding>>(
               finite_field, max_symbols, max_symbol_size);
}

template<class Stack>
using perpetual_decoder_binding =
    kodo_core::api::write_payload_binding<
    kodo_core::api::symbol_decoding_status_updater_binding<Stack>>;

kodoc_factory_t new_perpetual_decoder_factory(
    int32_t finite_field, uint32_t max_symbols, uint32_t max_symbol_size)
{
    return create_factory<
           runtime_decoder<
           kodo_rlnc::perpetual_decoder,
           perpetual_decoder_binding>>(
               finite_field, max_symbols, max_symbol_size);
}
}

#endif
