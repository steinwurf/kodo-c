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

#include "create_factory.hpp"

//------------------------------------------------------------------
// ENCODER FACTORY
//------------------------------------------------------------------

namespace
{
    template<class... Interfaces>
    struct encoder_interface_aggregator :
        kodo::api::build_interface,
        kodo::api::trace_interface,
        kodo::api::rank_interface,
        kodo::api::storage_interface,
        kodo::api::const_storage_interface,
        kodo::api::encoder_interface,
        kodo::api::write_payload_interface,
        kodo::api::payload_size_interface,
        Interfaces...
    {
        struct config_interface :
            kodo::api::build_interface::config_interface,
            kodo::api::trace_interface::config_interface,
            kodo::api::rank_interface::config_interface,
            kodo::api::storage_interface::config_interface,
            kodo::api::const_storage_interface::config_interface,
            kodo::api::encoder_interface::config_interface,
            kodo::api::write_payload_interface::config_interface,
            kodo::api::payload_size_interface::config_interface,
            Interfaces::config_interface...
        { };
    };

    using systematic_encoder_interface = encoder_interface_aggregator<
        kodo::api::systematic_interface>;

    using full_vector_encoder_interface = systematic_encoder_interface;

    using sparse_full_vector_encoder_interface = encoder_interface_aggregator<
        kodo::api::systematic_interface,
        kodo::api::sparse_encoder_interface>;

    using on_the_fly_encoder_interface = systematic_encoder_interface;

    using sliding_window_encoder_interface = encoder_interface_aggregator<
        kodo::api::systematic_interface,
        kodo::api::read_feedback_interface,
        kodo::api::feedback_size_interface>;

    using seed_encoder_interface = systematic_encoder_interface;

    using sparse_seed_encoder_interface = systematic_encoder_interface;

    using perpetual_encoder_interface = encoder_interface_aggregator<
        kodo::rlnc::api::perpetual_encoder_interface>;

    using fulcrum_encoder_interface = encoder_interface_aggregator<
        kodo::rlnc::api::fulcrum_interface>;
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
            full_vector_encoder,
            meta::typelist<storage_type<tag::const_shallow_storage>>,
            full_vector_encoder_interface>(
                finite_field, max_symbols, max_symbol_size);
    }
    else if (code_type == kodo_sparse_full_vector)
    {
        factory = create_factory<
            sparse_full_vector_encoder,
            meta::typelist<storage_type<tag::const_shallow_storage>>,
            sparse_full_vector_encoder_interface>(
                finite_field, max_symbols, max_symbol_size);
    }
    else if (code_type == kodo_on_the_fly)
    {
        factory = create_factory<
            on_the_fly_encoder,
            meta::typelist<storage_type<tag::const_shallow_storage>>,
            on_the_fly_encoder_interface>(
                finite_field, max_symbols, max_symbol_size);
    }
    else if (code_type == kodo_sliding_window)
    {
        factory = create_factory<
            sliding_window_encoder,
            meta::typelist<storage_type<tag::const_shallow_storage>>,
            sliding_window_encoder_interface>(
                finite_field, max_symbols, max_symbol_size);
    }
    else if (code_type == kodo_seed)
    {
        factory = create_factory<
            seed_encoder,
            meta::typelist<storage_type<tag::const_shallow_storage>>,
            seed_encoder_interface>(
                finite_field, max_symbols, max_symbol_size);
    }
    else if (code_type == kodo_sparse_seed)
    {
        factory = create_factory<
            sparse_seed_encoder,
            meta::typelist<storage_type<tag::const_shallow_storage>>,
            sparse_seed_encoder_interface>(
                finite_field, max_symbols, max_symbol_size);
    }
    else if (code_type == kodo_perpetual)
    {
        factory = create_factory<
            perpetual_encoder,
            meta::typelist<storage_type<tag::const_shallow_storage>>,
            perpetual_encoder_interface>(
                finite_field, max_symbols, max_symbol_size);
    }
    else if (code_type == kodo_fulcrum)
    {
        factory = create_factory<
            fulcrum_encoder,
            meta::typelist<storage_type<tag::const_shallow_storage>>,
            fulcrum_encoder_interface>(
                finite_field, max_symbols, max_symbol_size);
    }

    // Unknown code type
    assert(factory);

    return factory;
}
