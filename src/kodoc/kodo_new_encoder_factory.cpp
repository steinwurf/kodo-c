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

#include <kodo/runtime/select_field.hpp>
#include <kodo/runtime/use_shallow_encoder_storage.hpp>
#include <kodo/runtime/use_trace_enabled.hpp>
#include <kodo/runtime/final.hpp>

#include "create_factory_wrapper.hpp"

//------------------------------------------------------------------
// ENCODER FACTORY
//------------------------------------------------------------------

namespace
{
    template
    <
        template <class,class,class...> class Stack,
        class... AdditionalInterfaces
    >
    struct kodoc_runtime_encoder :
        kodo::runtime::select_field<
        kodo::runtime::use_shallow_encoder_storage<
        kodo::runtime::use_trace_enabled<
        kodo::runtime::final<Stack,
            kodo::api::storage_interface,
            kodo::api::encoder_interface,
            kodo::api::write_payload_interface,
            kodo::api::payload_size_interface,
            kodo::api::build_interface,
            AdditionalInterfaces...>>>>
    { };
}

kodo_factory_t
kodo_new_encoder_factory(int32_t code_type, int32_t finite_field,
                         uint32_t max_symbols, uint32_t max_symbol_size)
{
    using namespace kodo;
    using namespace kodo::rlnc;

    kodo_factory_t factory = 0;

    if (code_type == kodo_full_vector)
    {
        factory = create_factory<
            kodoc_runtime_encoder<
            full_vector_encoder,
            kodo::api::systematic_interface>>(
                finite_field, max_symbols, max_symbol_size);
    }
    else if (code_type == kodo_sparse_full_vector)
    {
        factory = create_factory<
            kodoc_runtime_encoder<
            sparse_full_vector_encoder,
            kodo::api::systematic_interface,
            kodo::api::sparse_encoder_interface>>(
                finite_field, max_symbols, max_symbol_size);
    }
    else if (code_type == kodo_on_the_fly)
    {
        factory = create_factory<
            kodoc_runtime_encoder<
            on_the_fly_encoder,
            kodo::api::systematic_interface>>(
                finite_field, max_symbols, max_symbol_size);
    }
    else if (code_type == kodo_sliding_window)
    {
        factory = create_factory<
            kodoc_runtime_encoder<
            sliding_window_encoder,
            kodo::api::systematic_interface,
            kodo::api::read_feedback_interface,
            kodo::api::feedback_size_interface>>(
                finite_field, max_symbols, max_symbol_size);
    }
    else if (code_type == kodo_seed)
    {
        factory = create_factory<
            kodoc_runtime_encoder<
            seed_encoder,
            kodo::api::systematic_interface>>(
                finite_field, max_symbols, max_symbol_size);
    }
    else if (code_type == kodo_sparse_seed)
    {
        factory = create_factory<
            kodoc_runtime_encoder<
            sparse_seed_encoder,
            kodo::api::systematic_interface>>(
                finite_field, max_symbols, max_symbol_size);
    }
    else if (code_type == kodo_perpetual)
    {
        factory = create_factory<
            kodoc_runtime_encoder<
            perpetual_encoder,
            kodo::rlnc::api::perpetual_encoder_interface>>(
                finite_field, max_symbols, max_symbol_size);
    }
    else if (code_type == kodo_fulcrum)
    {
        factory = create_factory<
            kodoc_runtime_encoder<
            fulcrum_encoder,
            kodo::rlnc::api::fulcrum_interface>>(
                finite_field, max_symbols, max_symbol_size);
    }

    // Unknown code type
    assert(factory);

    return factory;
}
