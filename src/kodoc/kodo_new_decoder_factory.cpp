// Copyright Steinwurf ApS 2014.
// Distributed under the "STEINWURF RESEARCH LICENSE 1.0".
// See accompanying file LICENSE.rst or
// http://www.steinwurf.com/licensing

#include "kodoc.h"

#include <cstdint>
#include <cassert>

#include <kodo/rlnc/full_vector_decoder.hpp>
#include <kodo/rlnc/on_the_fly_decoder.hpp>
#include <kodo/rlnc/sliding_window_decoder.hpp>
#include <kodo/rlnc/perpetual_decoder.hpp>
#include <kodo/rlnc/fulcrum_combined_decoder.hpp>
#include <kodo/rlnc/seed_decoder.hpp>
#include <kodo/rlnc/sparse_seed_decoder.hpp>

#include <kodo/api/api.hpp>
#include <kodo/rlnc/api/fulcrum_interface.hpp>

#include "create_factory_wrapper.hpp"

//------------------------------------------------------------------
// DECODER FACTORY FOR DEEP STORAGE STACKS
//------------------------------------------------------------------

kodo_factory_t
kodo_new_decoder_factory(int32_t code_type, int32_t finite_field,
                         uint32_t max_symbols, uint32_t max_symbol_size,
                         int32_t trace_mode, int32_t storage_mode)
{
    using namespace kodo;
    using namespace kodo::rlnc;

    kodo_factory_t factory = 0;

    if (code_type == kodo_full_vector)
    {
        factory = create_factory<
            full_vector_decoder,
            kodo::api::storage_interface,
            kodo::api::decoder_interface,
            kodo::api::read_payload_interface,
            kodo::api::payload_size_interface,
            kodo::api::build_interface,
            kodo::api::write_payload_interface>(
                finite_field, max_symbols, max_symbol_size, trace_mode,
                storage_mode);
    }
    // else if (code_type == kodo_on_the_fly)
    // {
    //     factory = create_factory<
    //         on_the_fly_decoder,
    //         kodo::api::partial_decoding_interface,
    //         kodo::api::write_payload_interface,
    //         kodo::api::storage_interface,
    //         kodo::api::decoder_interface,
    //         kodo::api::read_payload_interface,
    //         kodo::api::payload_size_interface,
    //         kodo::api::build_interface>(
    //             finite_field, max_symbols, max_symbol_size, trace_mode,
    //             storage_mode);
    // }
    // else if (code_type == kodo_sliding_window)
    // {
    //     factory = create_factory<
    //         sliding_window_decoder,
    //         kodo::api::partial_decoding_interface,
    //         kodo::api::write_payload_interface,
    //         kodo::api::write_feedback_interface,
    //         kodo::api::feedback_size_interface,
    //         kodo::api::storage_interface,
    //         kodo::api::decoder_interface,
    //         kodo::api::read_payload_interface,
    //         kodo::api::payload_size_interface,
    //         kodo::api::build_interface>(
    //             finite_field, max_symbols, max_symbol_size, trace_mode,
    //             storage_mode);
    // }
    // else if (code_type == kodo_seed)
    // {
    //     factory = create_factory<
    //         seed_decoder,
    //         kodo::api::storage_interface,
    //         kodo::api::decoder_interface,
    //         kodo::api::read_payload_interface,
    //         kodo::api::payload_size_interface,
    //         kodo::api::build_interface>(
    //             finite_field, max_symbols, max_symbol_size, trace_mode,
    //             storage_mode);
    // }
    // else if (code_type == kodo_sparse_seed)
    // {
    //     factory = create_factory<
    //         sparse_seed_decoder,
    //         kodo::api::storage_interface,
    //         kodo::api::decoder_interface,
    //         kodo::api::read_payload_interface,
    //         kodo::api::payload_size_interface,
    //         kodo::api::build_interface>(
    //             finite_field, max_symbols, max_symbol_size, trace_mode,
    //             storage_mode);
    // }
    // else if (code_type == kodo_perpetual)
    // {
    //     factory = create_factory<
    //         perpetual_decoder,
    //         kodo::api::write_payload_interface,
    //         kodo::api::storage_interface,
    //         kodo::api::decoder_interface,
    //         kodo::api::read_payload_interface,
    //         kodo::api::payload_size_interface,
    //         kodo::api::build_interface>(
    //             finite_field, max_symbols, max_symbol_size, trace_mode,
    //             storage_mode);
    // }
    // else if (code_type == kodo_fulcrum)
    // {
    //     factory = create_factory<
    //         fulcrum_combined_decoder,
    //         kodo::rlnc::api::fulcrum_interface,
    //         kodo::api::storage_interface,
    //         kodo::api::decoder_interface,
    //         kodo::api::read_payload_interface,
    //         kodo::api::payload_size_interface,
    //         kodo::api::build_interface>(
    //             finite_field, max_symbols, max_symbol_size, trace_mode,
    //             storage_mode);
    // }

    // Unknown code type
    assert(factory);

    return factory;
}
