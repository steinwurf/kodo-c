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

#include "create_factory.hpp"

//------------------------------------------------------------------
// DECODER FACTORY
//------------------------------------------------------------------
namespace
{
    template<class... Interfaces>
    struct decoder_interface_aggregator :
        kodo::api::build_interface,
        kodo::api::decoder_interface,
        kodo::api::mutable_storage_interface,
        kodo::api::payload_size_interface,
        kodo::api::rank_interface,
        kodo::api::read_payload_interface,
        kodo::api::storage_interface,
        kodo::api::trace_interface,
        Interfaces...
    {
        struct config_interface :
            kodo::api::build_interface::config_interface,
            kodo::api::decoder_interface::config_interface,
            kodo::api::mutable_storage_interface::config_interface,
            kodo::api::payload_size_interface::config_interface,
            kodo::api::rank_interface::config_interface,
            kodo::api::read_payload_interface::config_interface,
            kodo::api::storage_interface::config_interface,
            kodo::api::trace_interface::config_interface,
            Interfaces::config_interface...
        { };
    };

    using decoder_kodo_full_vector_interface = decoder_interface_aggregator<
        kodo::api::write_payload_interface>;

    using decoder_kodo_on_the_fly_interface = decoder_interface_aggregator<
        kodo::api::partial_decoding_interface,
        kodo::api::write_payload_interface>;

    using decoder_kodo_sliding_window_interface = decoder_interface_aggregator<
        kodo::api::partial_decoding_interface,
        kodo::api::write_payload_interface,
        kodo::api::write_feedback_interface,
        kodo::api::feedback_size_interface>;

    using decoder_kodo_seed_interface = decoder_interface_aggregator<>;

    using decoder_kodo_sparse_seed_interface = decoder_interface_aggregator<>;

    using decoder_kodo_perpetual_interface = decoder_interface_aggregator<
        kodo::api::write_payload_interface>;

    using decoder_kodo_fulcrum_interface = decoder_interface_aggregator<
        kodo::rlnc::api::fulcrum_interface>;
}

kodo_factory_t
kodo_new_decoder_factory(int32_t code_type, int32_t finite_field,
                         uint32_t max_symbols, uint32_t max_symbol_size)
{
    using namespace kodo;
    using namespace kodo::rlnc;

    kodo_factory_t factory = 0;

    if (code_type == kodo_full_vector)
    {
        factory = create_factory<
            full_vector_decoder,
            meta::typelist<storage_type<tag::mutable_shallow_storage>>,
            decoder_kodo_full_vector_interface>(
                finite_field, max_symbols, max_symbol_size);
    }
    else if (code_type == kodo_on_the_fly)
    {
        factory = create_factory<
            on_the_fly_decoder,
            meta::typelist<storage_type<tag::mutable_shallow_storage>>,
            decoder_kodo_on_the_fly_interface>(
                finite_field, max_symbols, max_symbol_size);
    }
    else if (code_type == kodo_sliding_window)
    {
        factory = create_factory<
            sliding_window_decoder,
            meta::typelist<storage_type<tag::mutable_shallow_storage>>,
            decoder_kodo_sliding_window_interface>(
                finite_field, max_symbols, max_symbol_size);
    }
    else if (code_type == kodo_seed)
    {
        factory = create_factory<
            seed_decoder,
            meta::typelist<storage_type<tag::mutable_shallow_storage>>,
            decoder_kodo_seed_interface>(
                finite_field, max_symbols, max_symbol_size);
    }
    else if (code_type == kodo_sparse_seed)
    {
        factory = create_factory<
            sparse_seed_decoder,
            meta::typelist<storage_type<tag::mutable_shallow_storage>>,
            decoder_kodo_sparse_seed_interface>(
                finite_field, max_symbols, max_symbol_size);
    }
    else if (code_type == kodo_perpetual)
    {
        factory = create_factory<
            perpetual_decoder,
            meta::typelist<storage_type<tag::mutable_shallow_storage>>,
            decoder_kodo_perpetual_interface>(
                finite_field, max_symbols, max_symbol_size);
    }
    else if (code_type == kodo_fulcrum)
    {
        factory = create_factory<
            fulcrum_combined_decoder,
            meta::typelist<storage_type<tag::mutable_shallow_storage>>,
            decoder_kodo_fulcrum_interface>(
                finite_field, max_symbols, max_symbol_size);
    }

    // Unknown code type
    assert(factory);

    return factory;
}
