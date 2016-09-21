// Copyright Steinwurf ApS 2014.
// Distributed under the "STEINWURF RESEARCH LICENSE 1.0".
// See accompanying file LICENSE.rst or
// http://www.steinwurf.com/licensing

#if !defined(KODOC_DISABLE_FULCRUM)

#include "kodoc.h"

#include <cstdint>

#include <kodo_fulcrum/api/fulcrum_binding.hpp>
#include <kodo_fulcrum/api/fulcrum_config_binding.hpp>
#include <kodo_fulcrum/api/fulcrum_nested_stack_binding.hpp>
#include <kodo_fulcrum/fulcrum_encoder.hpp>
#include <kodo_fulcrum/fulcrum_combined_decoder.hpp>

#include "create_factory.hpp"
#include "runtime_encoder.hpp"
#include "runtime_decoder.hpp"

namespace kodoc
{
template<class Stack>
using fulcrum_encoder_binding =
    kodo_fulcrum::api::fulcrum_binding<
    kodo_fulcrum::api::fulcrum_nested_stack_binding<Stack>>;

kodoc_factory_t new_fulcrum_encoder_factory(
    int32_t finite_field, uint32_t max_symbols, uint32_t max_symbol_size)
{
    return create_factory<
           runtime_encoder<
           kodo_fulcrum::fulcrum_encoder,
           fulcrum_encoder_binding,
           kodo_fulcrum::api::fulcrum_config_binding>>(
               finite_field, max_symbols, max_symbol_size);
}

kodoc_factory_t new_fulcrum_decoder_factory(
    int32_t finite_field, uint32_t max_symbols, uint32_t max_symbol_size)
{
    return create_factory<
           runtime_decoder<
           kodo_fulcrum::fulcrum_combined_decoder,
           kodo_fulcrum::api::fulcrum_binding,
           kodo_fulcrum::api::fulcrum_config_binding>>(
               finite_field, max_symbols, max_symbol_size);
}
}

#endif
