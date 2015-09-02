// Copyright Steinwurf ApS 2014.
// Distributed under the "STEINWURF RESEARCH LICENSE 1.0".
// See accompanying file LICENSE.rst or
// http://www.steinwurf.com/licensing

#include "kodoc.h"

#include <cstdint>
#include <cassert>

#include <kodo/rlnc/full_vector_encoder.hpp>
#include <kodo/rlnc/sparse_full_vector_encoder.hpp>
#include <kodo/rlnc/on_the_fly_encoder.hpp>
#include <kodo/rlnc/sliding_window_encoder.hpp>
#include <kodo/rlnc/perpetual_encoder.hpp>
#include <kodo/rlnc/fulcrum_encoder.hpp>
#include <kodo/rlnc/seed_encoder.hpp>
#include <kodo/rlnc/sparse_seed_encoder.hpp>

#include <kodo/api/api.hpp>
#include <kodo/rlnc/api/perpetual_encoder_interface.hpp>
#include <kodo/rlnc/api/fulcrum_interface.hpp>

#include "create_factory_wrapper.hpp"

//------------------------------------------------------------------
// ENCODER FACTORY FOR DEEP STORAGE STACKS
//------------------------------------------------------------------

kodo_factory_t
kodo_new_encoder_factory(int32_t code_type, int32_t finite_field,
                         uint32_t max_symbols, uint32_t max_symbol_size,
                         int32_t trace_mode, int32_t storage_mode)
{
    using namespace kodo;
    using namespace kodo::rlnc;

    kodo_factory_t factory = 0;

    if (code_type == kodo_full_vector)
    {
        factory = create_factory<
            kodo::runtime::runtime_encoder<
            full_vector_encoder,
            kodo::api::systematic_interface>>(
                finite_field, max_symbols, max_symbol_size, trace_mode,
                storage_mode);
    }
    else if (code_type == kodo_sparse_full_vector)
    {
        factory = create_factory<
            runtime::runtime_encoder<
            sparse_full_vector_encoder,
            kodo::api::systematic_interface,
            kodo::api::sparse_encoder_interface>>(
                finite_field, max_symbols, max_symbol_size, trace_mode,
                storage_mode);
    }
    else if (code_type == kodo_on_the_fly)
    {
        factory = create_factory<
            runtime::runtime_encoder<
            on_the_fly_encoder,
            kodo::api::systematic_interface>>(
                finite_field, max_symbols, max_symbol_size, trace_mode,
                storage_mode);
    }
    else if (code_type == kodo_sliding_window)
    {
        factory = create_factory<
            runtime::runtime_encoder<
            sliding_window_encoder,
            kodo::api::systematic_interface,
            kodo::api::read_feedback_interface,
            kodo::api::feedback_size_interface>>(
                finite_field, max_symbols, max_symbol_size, trace_mode,
                storage_mode);
    }
    else if (code_type == kodo_seed)
    {
        factory = create_factory<
            runtime::runtime_encoder<
            seed_encoder,
            kodo::api::systematic_interface>>(
                finite_field, max_symbols, max_symbol_size, trace_mode,
                storage_mode);
    }
    else if (code_type == kodo_sparse_seed)
    {
        factory = create_factory<
            runtime::runtime_encoder<
            sparse_seed_encoder,
            kodo::api::systematic_interface>>(
                finite_field, max_symbols, max_symbol_size, trace_mode,
                storage_mode);
    }
    else if (code_type == kodo_perpetual)
    {
        factory = create_factory<
            runtime::runtime_encoder<
            perpetual_encoder,
            kodo::rlnc::api::perpetual_encoder_interface>>(
                finite_field, max_symbols, max_symbol_size, trace_mode,
                storage_mode);
    }
    else if (code_type == kodo_fulcrum)
    {
        factory = create_factory<
            runtime::runtime_encoder<
            fulcrum_encoder,
            kodo::rlnc::api::fulcrum_interface>>(
                finite_field, max_symbols, max_symbol_size, trace_mode,
                storage_mode);
    }

    // Unknown code type
    assert(factory);

    return factory;
}
